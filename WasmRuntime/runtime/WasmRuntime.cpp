#include "WasmRuntime.h"
#include <mutex>
#include <cmath>

extern "C" void WASM_RT_ADD_PREFIX(init_globals_)(void);

static u32 getTotalMemory(void) {
    assert(Z_envZ_memory);
    return Z_envZ_memory->size;
}

static u32 enlargeMemory(void) {
    assert(Z_envZ_memory);
    wasm_rt_grow_memory(Z_envZ_memory, Z_envZ_memory->pages);
    return Z_envZ_memory->size;
}

/* import: 'env' 'memory' */
extern wasm_rt_memory_t(*Z_envZ_memory) = nullptr;
/* import: 'env' 'table' */
extern wasm_rt_table_t(*Z_envZ_table) = nullptr;
/* import: 'env' 'tableBase' */
extern u32(*Z_envZ_tableBaseZ_i) = nullptr;
/* import: 'env' 'DYNAMICTOP_PTR' */
extern u32(*Z_envZ_DYNAMICTOP_PTRZ_i) = nullptr;
/* import: 'env' 'STACKTOP' */
extern u32(*Z_envZ_STACKTOPZ_i) = nullptr;
/* import: 'env' 'abort' */
extern void (*Z_envZ_abortZ_vi)(u32) = nullptr;
/* import: 'env' 'enlargeMemory' */
extern u32 (*Z_envZ_enlargeMemoryZ_iv)(void) = &enlargeMemory;
/* import: 'env' 'getTotalMemory' */
extern u32 (*Z_envZ_getTotalMemoryZ_iv)(void) = &getTotalMemory;
/* import: 'env' 'abortOnCannotGrowMemory' */
extern u32 (*Z_envZ_abortOnCannotGrowMemoryZ_iv)(void) = nullptr;
/* import: 'env' '___setErrNo' */
extern void (*Z_envZ____setErrNoZ_vi)(u32) = nullptr;

namespace wasm {
    const size_t TOTAL_STACK = 1 * 1024 * 1024;  // 1M
    const size_t TOTAL_MEMORY = 8 * 1024 * 1024; // 8M
    const size_t TOTAL_STATIC = 1536;
    const size_t kWasmTableSize = 2;

    const size_t WASM_PAGE_SIZE = 65536;
    const size_t GLOBAL_BASE = 1024;
    const size_t STACK_ALIGN = 16;
    const size_t STATIC_BASE = GLOBAL_BASE;

    static size_t staticAlloc(size_t &STATICTOP, size_t size) {
        size_t ret = STATICTOP;
        STATICTOP = STATICTOP + size + 15 & -16;
        return ret;
    }

    static size_t alignMemory(size_t size, size_t factor) {
        size_t ret = std::ceil((double)size / (double)factor) * factor;
        return ret;
    }

    static size_t alignMemory(size_t size) {
        return alignMemory(size, STACK_ALIGN);
    }

    WasmRuntime::WasmRuntime() {
        size_t STATICTOP = STATIC_BASE + TOTAL_STATIC;
        const size_t DYNAMICTOP_PTR = staticAlloc(STATICTOP, 4);
        const size_t STACK_BASE = alignMemory(STATICTOP);
        const size_t STACKTOP = STACK_BASE;
        const size_t STACK_MAX = STACK_BASE + TOTAL_STACK;
        DYNAMIC_BASE = alignMemory(STACK_MAX);

        const int initPages = TOTAL_MEMORY / WASM_PAGE_SIZE;
        wasm_rt_allocate_memory(&memory, initPages, initPages * 8);
        wasm_rt_allocate_table(&table, kWasmTableSize, kWasmTableSize);

        tableBase = 0;
        stacktop = (u32)STACKTOP;
        dynamictop_ptr = (u32)DYNAMICTOP_PTR;
        nan = NAN;
        infinity = INFINITY;

        resetWasmEnv();
    }

    void WasmRuntime::switchWasmEnv() {
        Z_envZ_memory = &memory;
        Z_envZ_table = &table;
        Z_envZ_tableBaseZ_i = &tableBase;
        Z_envZ_STACKTOPZ_i = &stacktop;
        Z_envZ_DYNAMICTOP_PTRZ_i = &dynamictop_ptr;
        (*WASM_RT_ADD_PREFIX(init_globals_))();
    }

    WasmRuntime::~WasmRuntime() {
        if (memory.data) {
            free(memory.data);
            memory.data = nullptr;
        }

        if (table.data) {
            free(table.data);
            table.data = nullptr;
        }
    }

    static std::mutex g_wasmMutex;
    void WasmRuntime::resetWasmEnv() {
        std::lock_guard<std::mutex> lock(g_wasmMutex);
        switchWasmEnv();

        memset(memory.data, 0, memory.size);
        (*WASM_RT_ADD_PREFIX(init))();
        ((int32_t *)(memory.data))[dynamictop_ptr >> 2] = (int32_t)DYNAMIC_BASE;
    }

    void WasmRuntime::colorFromHexString(const char *hex, float *result) {
        std::lock_guard<std::mutex> lock(g_wasmMutex);
        switchWasmEnv();

        const u32 byteSize = (u32)strlen(hex) + 1;
        u32 ptr0 = (*Z__mallocZ_ii)(byteSize);
        u32 ptr1 = (*Z__mallocZ_ii)(sizeof(float) * 4);

        memcpy(wasm_ptr(ptr0), hex, byteSize);
        memcpy(wasm_ptr(ptr1), result, sizeof(float) * 4);

        (*Z__colorFromHexStringZ_vii)(ptr0, ptr1);

        memcpy(result, wasm_ptr(ptr1), sizeof(float) * 4);

        (*Z__freeZ_vi)(ptr0);
        (*Z__freeZ_vi)(ptr1);
    }

} // namespace wasm
