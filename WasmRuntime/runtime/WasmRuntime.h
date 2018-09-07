#ifndef __WASMRUNTIME_H__
#define __WASMRUNTIME_H__

#include "wasm_color.h"
#include <vector>

namespace wasm {
    class WasmRuntime {
    public:
        WasmRuntime();
        ~WasmRuntime();

        void colorFromHexString(const char *hex, float *result);
        void resetWasmEnv();

    private:
        void switchWasmEnv();
        u8 *wasm_ptr(u32 offset) {
            return (u8 *)(memory.data + offset);
        }

    private:
        wasm_rt_memory_t memory;
        wasm_rt_table_t table;

        size_t DYNAMIC_BASE;
        u32 stacktop;
        u32 dynamictop_ptr;
        f64 nan;
        f64 infinity;
        u32 tableBase;
    };
} // namespace wasm

#endif
