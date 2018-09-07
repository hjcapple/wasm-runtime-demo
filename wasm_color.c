#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "wasm_color.h"
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define TRAP(x) (wasm_rt_trap(WASM_RT_TRAP_##x), 0)

#define FUNC_PROLOGUE                                            \
  if (++wasm_rt_call_stack_depth > WASM_RT_MAX_CALL_STACK_DEPTH) \
    TRAP(EXHAUSTION)

#define FUNC_EPILOGUE --wasm_rt_call_stack_depth

#define UNREACHABLE TRAP(UNREACHABLE)

#define CALL_INDIRECT(table, t, ft, x, ...)          \
  (LIKELY((x) < table.size && table.data[x].func &&  \
          table.data[x].func_type == func_types[ft]) \
       ? ((t)table.data[x].func)(__VA_ARGS__)        \
       : TRAP(CALL_INDIRECT))

#define MEMCHECK(mem, a, t)  \
  if (UNLIKELY((a) + sizeof(t) > mem->size)) TRAP(OOB)

#define DEFINE_LOAD(name, t1, t2, t3)              \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {   \
    MEMCHECK(mem, addr, t1);                       \
    t1 result;                                     \
    memcpy(&result, &mem->data[addr], sizeof(t1)); \
    return (t3)(t2)result;                         \
  }

#define DEFINE_STORE(name, t1, t2)                           \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) { \
    MEMCHECK(mem, addr, t1);                                 \
    t1 wrapped = (t1)value;                                  \
    memcpy(&mem->data[addr], &wrapped, sizeof(t1));          \
  }

DEFINE_LOAD(i32_load, u32, u32, u32);
DEFINE_LOAD(i64_load, u64, u64, u64);
DEFINE_LOAD(f32_load, f32, f32, f32);
DEFINE_LOAD(f64_load, f64, f64, f64);
DEFINE_LOAD(i32_load8_s, s8, s32, u32);
DEFINE_LOAD(i64_load8_s, s8, s64, u64);
DEFINE_LOAD(i32_load8_u, u8, u32, u32);
DEFINE_LOAD(i64_load8_u, u8, u64, u64);
DEFINE_LOAD(i32_load16_s, s16, s32, u32);
DEFINE_LOAD(i64_load16_s, s16, s64, u64);
DEFINE_LOAD(i32_load16_u, u16, u32, u32);
DEFINE_LOAD(i64_load16_u, u16, u64, u64);
DEFINE_LOAD(i64_load32_s, s32, s64, u64);
DEFINE_LOAD(i64_load32_u, u32, u64, u64);
DEFINE_STORE(i32_store, u32, u32);
DEFINE_STORE(i64_store, u64, u64);
DEFINE_STORE(f32_store, f32, f32);
DEFINE_STORE(f64_store, f64, f64);
DEFINE_STORE(i32_store8, u8, u32);
DEFINE_STORE(i32_store16, u16, u32);
DEFINE_STORE(i64_store8, u8, u64);
DEFINE_STORE(i64_store16, u16, u64);
DEFINE_STORE(i64_store32, u32, u64);

#define I32_CLZ(x) ((x) ? __builtin_clz(x) : 32)
#define I64_CLZ(x) ((x) ? __builtin_clzll(x) : 64)
#define I32_CTZ(x) ((x) ? __builtin_ctz(x) : 32)
#define I64_CTZ(x) ((x) ? __builtin_ctzll(x) : 64)
#define I32_POPCNT(x) (__builtin_popcount(x))
#define I64_POPCNT(x) (__builtin_popcountll(x))

#define DIV_S(ut, min, x, y)                                 \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO)  \
  : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \
  : (ut)((x) / (y)))

#define REM_S(ut, min, x, y)                                \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO) \
  : (UNLIKELY((x) == min && (y) == -1)) ? 0                 \
  : (ut)((x) % (y)))

#define I32_DIV_S(x, y) DIV_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_DIV_S(x, y) DIV_S(u64, INT64_MIN, (s64)x, (s64)y)
#define I32_REM_S(x, y) REM_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_REM_S(x, y) REM_S(u64, INT64_MIN, (s64)x, (s64)y)

#define DIVREM_U(op, x, y) \
  ((UNLIKELY((y) == 0)) ? TRAP(DIV_BY_ZERO) : ((x) op (y)))

#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)

#define ROTL(x, y, mask) \
  (((x) << ((y) & (mask))) | ((x) >> (((mask) - (y) + 1) & (mask))))
#define ROTR(x, y, mask) \
  (((x) >> ((y) & (mask))) | ((x) << (((mask) - (y) + 1) & (mask))))

#define I32_ROTL(x, y) ROTL(x, y, 31)
#define I64_ROTL(x, y) ROTL(x, y, 63)
#define I32_ROTR(x, y) ROTR(x, y, 31)
#define I64_ROTR(x, y) ROTR(x, y, 63)

#define FMIN(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? x : y) \
  : (x < y) ? x : y)

#define FMAX(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? y : x) \
  : (x > y) ? x : y)

#define TRUNC_S(ut, st, ft, min, max, maxop, x)                             \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                       \
  : (UNLIKELY((x) < (ft)(min) || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(st)(x))

#define I32_TRUNC_S_F32(x) TRUNC_S(u32, s32, f32, INT32_MIN, INT32_MAX, >=, x)
#define I64_TRUNC_S_F32(x) TRUNC_S(u64, s64, f32, INT64_MIN, INT64_MAX, >=, x)
#define I32_TRUNC_S_F64(x) TRUNC_S(u32, s32, f64, INT32_MIN, INT32_MAX, >,  x)
#define I64_TRUNC_S_F64(x) TRUNC_S(u64, s64, f64, INT64_MIN, INT64_MAX, >=, x)

#define TRUNC_U(ut, ft, max, maxop, x)                                    \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                     \
  : (UNLIKELY((x) <= (ft)-1 || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(x))

#define I32_TRUNC_U_F32(x) TRUNC_U(u32, f32, UINT32_MAX, >=, x)
#define I64_TRUNC_U_F32(x) TRUNC_U(u64, f32, UINT64_MAX, >=, x)
#define I32_TRUNC_U_F64(x) TRUNC_U(u32, f64, UINT32_MAX, >,  x)
#define I64_TRUNC_U_F64(x) TRUNC_U(u64, f64, UINT64_MAX, >=, x)

#define DEFINE_REINTERPRET(name, t1, t2)  \
  static inline t2 name(t1 x) {           \
    t2 result;                            \
    memcpy(&result, &x, sizeof(result));  \
    return result;                        \
  }

DEFINE_REINTERPRET(f32_reinterpret_i32, u32, f32)
DEFINE_REINTERPRET(i32_reinterpret_f32, f32, u32)
DEFINE_REINTERPRET(f64_reinterpret_i64, u64, f64)
DEFINE_REINTERPRET(i64_reinterpret_f64, f64, u64)


static u32 func_types[4];

static void init_func_types(void) {
  func_types[0] = wasm_rt_register_func_type(1, 1, WASM_RT_I32, WASM_RT_I32);
  func_types[1] = wasm_rt_register_func_type(1, 0, WASM_RT_I32);
  func_types[2] = wasm_rt_register_func_type(0, 1, WASM_RT_I32);
  func_types[3] = wasm_rt_register_func_type(2, 0, WASM_RT_I32, WASM_RT_I32);
}

static u32 f5(u32);
static u32 _malloc(u32);
static void _colorFromHexString(u32, u32);
static u32 f8(u32);
static u32 f9(u32);
static u32 f10(u32);
static u32 ___errno_location(void);
static void _free(u32);

static u32 g3;
static u32 g4;

static void init_globals(void) {
  g3 = (*Z_envZ_DYNAMICTOP_PTRZ_i);
  g4 = (*Z_envZ_STACKTOPZ_i);
}

static u32 f5(u32 p0) {
  u32 l0 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2;
  i0 = p0;
  i1 = 0u;
  i0 = (u32)((s32)i0 > (s32)i1);
  i1 = g3;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  l0 = i1;
  i2 = p0;
  i1 += i2;
  p0 = i1;
  i2 = l0;
  i1 = (u32)((s32)i1 < (s32)i2);
  i0 &= i1;
  i1 = p0;
  i2 = 0u;
  i1 = (u32)((s32)i1 < (s32)i2);
  i0 |= i1;
  if (i0) {
    i0 = (*Z_envZ_abortOnCannotGrowMemoryZ_iv)();
    i0 = 12u;
    (*Z_envZ____setErrNoZ_vi)(i0);
    i0 = 4294967295u;
    goto Bfunc;
  }
  i0 = g3;
  i1 = p0;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  i0 = p0;
  i1 = (*Z_envZ_getTotalMemoryZ_iv)();
  i0 = (u32)((s32)i0 > (s32)i1);
  if (i0) {
    i0 = (*Z_envZ_enlargeMemoryZ_iv)();
    i0 = !(i0);
    if (i0) {
      i0 = g3;
      i1 = l0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 12u;
      (*Z_envZ____setErrNoZ_vi)(i0);
      i0 = 4294967295u;
      goto Bfunc;
    }
  }
  i0 = l0;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static u32 _malloc(u32 p0) {
  u32 l0 = 0, l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, 
      l8 = 0, l9 = 0, l10 = 0, l11 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3, i4;
  u64 j1;
  i0 = g4;
  l0 = i0;
  i0 = g4;
  i1 = 16u;
  i0 += i1;
  g4 = i0;
  i0 = l0;
  l9 = i0;
  i0 = p0;
  i1 = 245u;
  i0 = i0 < i1;
  if (i0) {
    i0 = p0;
    i1 = 11u;
    i0 += i1;
    i1 = 4294967288u;
    i0 &= i1;
    l0 = i0;
    i0 = 1028u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l5 = i0;
    i1 = p0;
    i2 = 11u;
    i1 = i1 < i2;
    if (i1) {
      i1 = 16u;
    } else {
      i1 = l0;
    }
    p0 = i1;
    i2 = 3u;
    i1 >>= (i2 & 31);
    l0 = i1;
    i0 >>= (i1 & 31);
    l1 = i0;
    i1 = 3u;
    i0 &= i1;
    if (i0) {
      i0 = l1;
      i1 = 1u;
      i0 &= i1;
      i1 = 1u;
      i0 ^= i1;
      i1 = l0;
      i0 += i1;
      p0 = i0;
      i1 = 3u;
      i0 <<= (i1 & 31);
      i1 = 1068u;
      i0 += i1;
      l0 = i0;
      i1 = 8u;
      i0 += i1;
      l3 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l1 = i0;
      i1 = 8u;
      i0 += i1;
      l4 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l2 = i0;
      i1 = l0;
      i0 = i0 == i1;
      if (i0) {
        i0 = 1028u;
        i1 = l5;
        i2 = 1u;
        i3 = p0;
        i2 <<= (i3 & 31);
        i3 = 4294967295u;
        i2 ^= i3;
        i1 &= i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
      } else {
        i0 = l2;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l3;
        i1 = l2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
      }
      i0 = l1;
      i1 = p0;
      i2 = 3u;
      i1 <<= (i2 & 31);
      p0 = i1;
      i2 = 3u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l1;
      i1 = p0;
      i0 += i1;
      i1 = 4u;
      i0 += i1;
      p0 = i0;
      i1 = p0;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l9;
      g4 = i0;
      i0 = l4;
      goto Bfunc;
    }
    i0 = p0;
    i1 = 1036u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    l6 = i1;
    i0 = i0 > i1;
    if (i0) {
      i0 = l1;
      if (i0) {
        i0 = l1;
        i1 = l0;
        i0 <<= (i1 & 31);
        i1 = 2u;
        i2 = l0;
        i1 <<= (i2 & 31);
        l0 = i1;
        i2 = 0u;
        i3 = l0;
        i2 -= i3;
        i1 |= i2;
        i0 &= i1;
        l0 = i0;
        i1 = 0u;
        i2 = l0;
        i1 -= i2;
        i0 &= i1;
        i1 = 4294967295u;
        i0 += i1;
        l1 = i0;
        i1 = 12u;
        i0 >>= (i1 & 31);
        i1 = 16u;
        i0 &= i1;
        l0 = i0;
        i0 = l1;
        i1 = l0;
        i0 >>= (i1 & 31);
        l1 = i0;
        i1 = 5u;
        i0 >>= (i1 & 31);
        i1 = 8u;
        i0 &= i1;
        l2 = i0;
        i1 = l0;
        i0 |= i1;
        i1 = l1;
        i2 = l2;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 2u;
        i1 >>= (i2 & 31);
        i2 = 4u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 1u;
        i1 >>= (i2 & 31);
        i2 = 2u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 1u;
        i1 >>= (i2 & 31);
        i2 = 1u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        i0 += i1;
        l2 = i0;
        i1 = 3u;
        i0 <<= (i1 & 31);
        i1 = 1068u;
        i0 += i1;
        l0 = i0;
        i1 = 8u;
        i0 += i1;
        l4 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l1 = i0;
        i1 = 8u;
        i0 += i1;
        l7 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l3 = i0;
        i1 = l0;
        i0 = i0 == i1;
        if (i0) {
          i0 = 1028u;
          i1 = l5;
          i2 = 1u;
          i3 = l2;
          i2 <<= (i3 & 31);
          i3 = 4294967295u;
          i2 ^= i3;
          i1 &= i2;
          l0 = i1;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
        } else {
          i0 = l3;
          i1 = l0;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          i0 = l4;
          i1 = l3;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l5;
          l0 = i0;
        }
        i0 = l1;
        i1 = p0;
        i2 = 3u;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = l1;
        i1 = p0;
        i0 += i1;
        l5 = i0;
        i1 = l2;
        i2 = 3u;
        i1 <<= (i2 & 31);
        l2 = i1;
        i2 = p0;
        i1 -= i2;
        l3 = i1;
        i2 = 1u;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = l1;
        i1 = l2;
        i0 += i1;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l6;
        if (i0) {
          i0 = 1048u;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          l2 = i0;
          i0 = l6;
          i1 = 3u;
          i0 >>= (i1 & 31);
          l1 = i0;
          i1 = 3u;
          i0 <<= (i1 & 31);
          i1 = 1068u;
          i0 += i1;
          p0 = i0;
          i0 = l0;
          i1 = 1u;
          i2 = l1;
          i1 <<= (i2 & 31);
          l1 = i1;
          i0 &= i1;
          if (i0) {
            i0 = p0;
            i1 = 8u;
            i0 += i1;
            l1 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
          } else {
            i0 = 1028u;
            i1 = l0;
            i2 = l1;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
            i0 = p0;
            i1 = 8u;
            i0 += i1;
            l1 = i0;
            i0 = p0;
          }
          l0 = i0;
          i0 = l1;
          i1 = l2;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l0;
          i1 = l2;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          i0 = l2;
          i1 = l0;
          i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
          i0 = l2;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        }
        i0 = 1036u;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = 1048u;
        i1 = l5;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l9;
        g4 = i0;
        i0 = l7;
        goto Bfunc;
      }
      i0 = 1032u;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l11 = i0;
      if (i0) {
        i0 = l11;
        i1 = 0u;
        i2 = l11;
        i1 -= i2;
        i0 &= i1;
        i1 = 4294967295u;
        i0 += i1;
        l1 = i0;
        i1 = 12u;
        i0 >>= (i1 & 31);
        i1 = 16u;
        i0 &= i1;
        l0 = i0;
        i0 = l1;
        i1 = l0;
        i0 >>= (i1 & 31);
        l1 = i0;
        i1 = 5u;
        i0 >>= (i1 & 31);
        i1 = 8u;
        i0 &= i1;
        l2 = i0;
        i1 = l0;
        i0 |= i1;
        i1 = l1;
        i2 = l2;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 2u;
        i1 >>= (i2 & 31);
        i2 = 4u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 1u;
        i1 >>= (i2 & 31);
        i2 = 2u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        l0 = i1;
        i2 = 1u;
        i1 >>= (i2 & 31);
        i2 = 1u;
        i1 &= i2;
        l1 = i1;
        i0 |= i1;
        i1 = l0;
        i2 = l1;
        i1 >>= (i2 & 31);
        i0 += i1;
        i1 = 2u;
        i0 <<= (i1 & 31);
        i1 = 1332u;
        i0 += i1;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l2 = i0;
        l0 = i0;
        i0 = l2;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
        i1 = 4294967288u;
        i0 &= i1;
        i1 = p0;
        i0 -= i1;
        l3 = i0;
        L14: 
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 16));
          l1 = i0;
          if (i0) {
            i0 = l1;
            l0 = i0;
          } else {
            i0 = l0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 20));
            l0 = i0;
            i0 = !(i0);
            if (i0) {goto B15;}
          }
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
          i1 = 4294967288u;
          i0 &= i1;
          i1 = p0;
          i0 -= i1;
          l1 = i0;
          i1 = l3;
          i0 = i0 < i1;
          l4 = i0;
          i0 = !(i0);
          if (i0) {
            i0 = l3;
            l1 = i0;
          }
          i0 = l4;
          if (i0) {
            i0 = l0;
            l2 = i0;
          }
          i0 = l1;
          l3 = i0;
          goto L14;
          B15:;
        i0 = l2;
        i1 = p0;
        i0 += i1;
        l10 = i0;
        i1 = l2;
        i0 = i0 > i1;
        if (i0) {
          i0 = l2;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 24));
          l8 = i0;
          i0 = l2;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
          l0 = i0;
          i1 = l2;
          i0 = i0 == i1;
          if (i0) {
            i0 = l2;
            i1 = 20u;
            i0 += i1;
            l1 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
            l0 = i0;
            i0 = !(i0);
            if (i0) {
              i0 = l2;
              i1 = 16u;
              i0 += i1;
              l1 = i0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              l0 = i0;
              i0 = !(i0);
              if (i0) {
                i0 = 0u;
                l0 = i0;
                goto B20;
              }
            }
            L24: 
              i0 = l0;
              i1 = 20u;
              i0 += i1;
              l4 = i0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              l7 = i0;
              i0 = !(i0);
              if (i0) {
                i0 = l0;
                i1 = 16u;
                i0 += i1;
                l4 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                l7 = i0;
                i0 = !(i0);
                if (i0) {goto B25;}
              }
              i0 = l4;
              l1 = i0;
              i0 = l7;
              l0 = i0;
              goto L24;
              B25:;
            i0 = l1;
            i1 = 0u;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
          } else {
            i0 = l2;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
            l1 = i0;
            i1 = l0;
            i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
            i0 = l0;
            i1 = l1;
            i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
          }
          B20:;
          i0 = l8;
          if (i0) {
            i0 = l2;
            i1 = l2;
            i1 = i32_load(Z_envZ_memory, (u64)(i1 + 28));
            l1 = i1;
            i2 = 2u;
            i1 <<= (i2 & 31);
            i2 = 1332u;
            i1 += i2;
            l4 = i1;
            i1 = i32_load(Z_envZ_memory, (u64)(i1));
            i0 = i0 == i1;
            if (i0) {
              i0 = l4;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              i0 = l0;
              i0 = !(i0);
              if (i0) {
                i0 = 1032u;
                i1 = l11;
                i2 = 1u;
                i3 = l1;
                i2 <<= (i3 & 31);
                i3 = 4294967295u;
                i2 ^= i3;
                i1 &= i2;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
                goto B27;
              }
            } else {
              i0 = l8;
              i1 = 20u;
              i0 += i1;
              l1 = i0;
              i0 = l8;
              i1 = 16u;
              i0 += i1;
              l4 = i0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              i1 = l2;
              i0 = i0 == i1;
              if (i0) {
                i0 = l4;
              } else {
                i0 = l1;
              }
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              i0 = l0;
              i0 = !(i0);
              if (i0) {goto B27;}
            }
            i0 = l0;
            i1 = l8;
            i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
            i0 = l2;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 16));
            l1 = i0;
            if (i0) {
              i0 = l0;
              i1 = l1;
              i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
              i0 = l1;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
            }
            i0 = l2;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 20));
            l1 = i0;
            if (i0) {
              i0 = l0;
              i1 = l1;
              i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
              i0 = l1;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
            }
          }
          B27:;
          i0 = l3;
          i1 = 16u;
          i0 = i0 < i1;
          if (i0) {
            i0 = l2;
            i1 = l3;
            i2 = p0;
            i1 += i2;
            p0 = i1;
            i2 = 3u;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
            i0 = l2;
            i1 = p0;
            i0 += i1;
            i1 = 4u;
            i0 += i1;
            p0 = i0;
            i1 = p0;
            i1 = i32_load(Z_envZ_memory, (u64)(i1));
            i2 = 1u;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
          } else {
            i0 = l2;
            i1 = p0;
            i2 = 3u;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
            i0 = l10;
            i1 = l3;
            i2 = 1u;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
            i0 = l10;
            i1 = l3;
            i0 += i1;
            i1 = l3;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
            i0 = l6;
            if (i0) {
              i0 = 1048u;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              l4 = i0;
              i0 = l6;
              i1 = 3u;
              i0 >>= (i1 & 31);
              l0 = i0;
              i1 = 3u;
              i0 <<= (i1 & 31);
              i1 = 1068u;
              i0 += i1;
              p0 = i0;
              i0 = 1u;
              i1 = l0;
              i0 <<= (i1 & 31);
              l0 = i0;
              i1 = l5;
              i0 &= i1;
              if (i0) {
                i0 = p0;
                i1 = 8u;
                i0 += i1;
                l1 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
              } else {
                i0 = 1028u;
                i1 = l0;
                i2 = l5;
                i1 |= i2;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
                i0 = p0;
                i1 = 8u;
                i0 += i1;
                l1 = i0;
                i0 = p0;
              }
              l0 = i0;
              i0 = l1;
              i1 = l4;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              i0 = l0;
              i1 = l4;
              i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
              i0 = l4;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
              i0 = l4;
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
            }
            i0 = 1036u;
            i1 = l3;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
            i0 = 1048u;
            i1 = l10;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
          }
          i0 = l9;
          g4 = i0;
          i0 = l2;
          i1 = 8u;
          i0 += i1;
          goto Bfunc;
        }
      }
    }
  } else {
    i0 = p0;
    i1 = 4294967231u;
    i0 = i0 > i1;
    if (i0) {
      i0 = 4294967295u;
      p0 = i0;
    } else {
      i0 = p0;
      i1 = 11u;
      i0 += i1;
      l0 = i0;
      i1 = 4294967288u;
      i0 &= i1;
      p0 = i0;
      i0 = 1032u;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l3 = i0;
      if (i0) {
        i0 = l0;
        i1 = 8u;
        i0 >>= (i1 & 31);
        l0 = i0;
        if (i0) {
          i0 = p0;
          i1 = 16777215u;
          i0 = i0 > i1;
          if (i0) {
            i0 = 31u;
          } else {
            i0 = p0;
            i1 = 14u;
            i2 = l0;
            i3 = l0;
            i4 = 1048320u;
            i3 += i4;
            i4 = 16u;
            i3 >>= (i4 & 31);
            i4 = 8u;
            i3 &= i4;
            l0 = i3;
            i2 <<= (i3 & 31);
            l1 = i2;
            i3 = 520192u;
            i2 += i3;
            i3 = 16u;
            i2 >>= (i3 & 31);
            i3 = 4u;
            i2 &= i3;
            l2 = i2;
            i3 = l0;
            i2 |= i3;
            i3 = l1;
            i4 = l2;
            i3 <<= (i4 & 31);
            l0 = i3;
            i4 = 245760u;
            i3 += i4;
            i4 = 16u;
            i3 >>= (i4 & 31);
            i4 = 2u;
            i3 &= i4;
            l1 = i3;
            i2 |= i3;
            i1 -= i2;
            i2 = l0;
            i3 = l1;
            i2 <<= (i3 & 31);
            i3 = 15u;
            i2 >>= (i3 & 31);
            i1 += i2;
            l0 = i1;
            i2 = 7u;
            i1 += i2;
            i0 >>= (i1 & 31);
            i1 = 1u;
            i0 &= i1;
            i1 = l0;
            i2 = 1u;
            i1 <<= (i2 & 31);
            i0 |= i1;
          }
        } else {
          i0 = 0u;
        }
        l6 = i0;
        i0 = 0u;
        i1 = p0;
        i0 -= i1;
        l2 = i0;
        i0 = l6;
        i1 = 2u;
        i0 <<= (i1 & 31);
        i1 = 1332u;
        i0 += i1;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l0 = i0;
        if (i0) {
          i0 = 25u;
          i1 = l6;
          i2 = 1u;
          i1 >>= (i2 & 31);
          i0 -= i1;
          l5 = i0;
          i0 = 0u;
          l1 = i0;
          i0 = p0;
          i1 = l6;
          i2 = 31u;
          i1 = i1 == i2;
          if (i1) {
            i1 = 0u;
          } else {
            i1 = l5;
          }
          i0 <<= (i1 & 31);
          l4 = i0;
          i0 = 0u;
          l5 = i0;
          L45: 
            i0 = l0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
            i1 = 4294967288u;
            i0 &= i1;
            i1 = p0;
            i0 -= i1;
            l7 = i0;
            i1 = l2;
            i0 = i0 < i1;
            if (i0) {
              i0 = l7;
              if (i0) {
                i0 = l7;
                l2 = i0;
                i0 = l0;
              } else {
                i0 = 0u;
                l2 = i0;
                i0 = l0;
                l1 = i0;
                goto B42;
              }
              l1 = i0;
            }
            i0 = l0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 20));
            l7 = i0;
            i0 = !(i0);
            i1 = l7;
            i2 = l0;
            i3 = 16u;
            i2 += i3;
            i3 = l4;
            i4 = 31u;
            i3 >>= (i4 & 31);
            i4 = 2u;
            i3 <<= (i4 & 31);
            i2 += i3;
            i2 = i32_load(Z_envZ_memory, (u64)(i2));
            l0 = i2;
            i1 = i1 == i2;
            i0 |= i1;
            i0 = !(i0);
            if (i0) {
              i0 = l7;
              l5 = i0;
            }
            i0 = l4;
            i1 = 1u;
            i0 <<= (i1 & 31);
            l4 = i0;
            i0 = l0;
            if (i0) {goto L45;}
          i0 = l1;
        } else {
          i0 = 0u;
        }
        l0 = i0;
        i0 = l5;
        i1 = l0;
        i0 |= i1;
        if (i0) {
          i0 = l5;
        } else {
          i0 = 2u;
          i1 = l6;
          i0 <<= (i1 & 31);
          l0 = i0;
          i1 = 0u;
          i2 = l0;
          i1 -= i2;
          i0 |= i1;
          i1 = l3;
          i0 &= i1;
          l0 = i0;
          i0 = !(i0);
          if (i0) {goto B3;}
          i0 = l0;
          i1 = 0u;
          i2 = l0;
          i1 -= i2;
          i0 &= i1;
          i1 = 4294967295u;
          i0 += i1;
          l5 = i0;
          i1 = 12u;
          i0 >>= (i1 & 31);
          i1 = 16u;
          i0 &= i1;
          l1 = i0;
          i0 = 0u;
          l0 = i0;
          i0 = l5;
          i1 = l1;
          i0 >>= (i1 & 31);
          l5 = i0;
          i1 = 5u;
          i0 >>= (i1 & 31);
          i1 = 8u;
          i0 &= i1;
          l4 = i0;
          i1 = l1;
          i0 |= i1;
          i1 = l5;
          i2 = l4;
          i1 >>= (i2 & 31);
          l1 = i1;
          i2 = 2u;
          i1 >>= (i2 & 31);
          i2 = 4u;
          i1 &= i2;
          l5 = i1;
          i0 |= i1;
          i1 = l1;
          i2 = l5;
          i1 >>= (i2 & 31);
          l1 = i1;
          i2 = 1u;
          i1 >>= (i2 & 31);
          i2 = 2u;
          i1 &= i2;
          l5 = i1;
          i0 |= i1;
          i1 = l1;
          i2 = l5;
          i1 >>= (i2 & 31);
          l1 = i1;
          i2 = 1u;
          i1 >>= (i2 & 31);
          i2 = 1u;
          i1 &= i2;
          l5 = i1;
          i0 |= i1;
          i1 = l1;
          i2 = l5;
          i1 >>= (i2 & 31);
          i0 += i1;
          i1 = 2u;
          i0 <<= (i1 & 31);
          i1 = 1332u;
          i0 += i1;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
        }
        l1 = i0;
        if (i0) {goto B42;}
        i0 = l0;
        l5 = i0;
        goto B41;
        B42:;
        i0 = l0;
        l4 = i0;
        i0 = l1;
        l0 = i0;
        L50: 
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
          l1 = i0;
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 16));
          l5 = i0;
          i0 = !(i0);
          if (i0) {
            i0 = l0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 20));
            l5 = i0;
          }
          i0 = l1;
          i1 = 4294967288u;
          i0 &= i1;
          i1 = p0;
          i0 -= i1;
          l1 = i0;
          i1 = l2;
          i0 = i0 < i1;
          l7 = i0;
          i0 = !(i0);
          if (i0) {
            i0 = l2;
            l1 = i0;
          }
          i0 = l7;
          i0 = !(i0);
          if (i0) {
            i0 = l4;
            l0 = i0;
          }
          i0 = l5;
          if (i0) {
            i0 = l0;
            l4 = i0;
            i0 = l1;
            l2 = i0;
            i0 = l5;
            l0 = i0;
            goto L50;
          } else {
            i0 = l0;
            l5 = i0;
            i0 = l1;
          }
          l2 = i0;
        B41:;
        i0 = l5;
        if (i0) {
          i0 = l2;
          i1 = 1036u;
          i1 = i32_load(Z_envZ_memory, (u64)(i1));
          i2 = p0;
          i1 -= i2;
          i0 = i0 < i1;
          if (i0) {
            i0 = l5;
            i1 = p0;
            i0 += i1;
            l6 = i0;
            i1 = l5;
            i0 = i0 > i1;
            if (i0) {
              i0 = l5;
              i0 = i32_load(Z_envZ_memory, (u64)(i0 + 24));
              l8 = i0;
              i0 = l5;
              i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
              l0 = i0;
              i1 = l5;
              i0 = i0 == i1;
              if (i0) {
                i0 = l5;
                i1 = 20u;
                i0 += i1;
                l1 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                l0 = i0;
                i0 = !(i0);
                if (i0) {
                  i0 = l5;
                  i1 = 16u;
                  i0 += i1;
                  l1 = i0;
                  i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  l0 = i0;
                  i0 = !(i0);
                  if (i0) {
                    i0 = 0u;
                    l0 = i0;
                    goto B58;
                  }
                }
                L62: 
                  i0 = l0;
                  i1 = 20u;
                  i0 += i1;
                  l4 = i0;
                  i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  l7 = i0;
                  i0 = !(i0);
                  if (i0) {
                    i0 = l0;
                    i1 = 16u;
                    i0 += i1;
                    l4 = i0;
                    i0 = i32_load(Z_envZ_memory, (u64)(i0));
                    l7 = i0;
                    i0 = !(i0);
                    if (i0) {goto B63;}
                  }
                  i0 = l4;
                  l1 = i0;
                  i0 = l7;
                  l0 = i0;
                  goto L62;
                  B63:;
                i0 = l1;
                i1 = 0u;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
              } else {
                i0 = l5;
                i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
                l1 = i0;
                i1 = l0;
                i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                i0 = l0;
                i1 = l1;
                i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
              }
              B58:;
              i0 = l8;
              if (i0) {
                i0 = l5;
                i1 = l5;
                i1 = i32_load(Z_envZ_memory, (u64)(i1 + 28));
                l1 = i1;
                i2 = 2u;
                i1 <<= (i2 & 31);
                i2 = 1332u;
                i1 += i2;
                l4 = i1;
                i1 = i32_load(Z_envZ_memory, (u64)(i1));
                i0 = i0 == i1;
                if (i0) {
                  i0 = l4;
                  i1 = l0;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = l0;
                  i0 = !(i0);
                  if (i0) {
                    i0 = 1032u;
                    i1 = l3;
                    i2 = 1u;
                    i3 = l1;
                    i2 <<= (i3 & 31);
                    i3 = 4294967295u;
                    i2 ^= i3;
                    i1 &= i2;
                    l0 = i1;
                    i32_store(Z_envZ_memory, (u64)(i0), i1);
                    goto B65;
                  }
                } else {
                  i0 = l8;
                  i1 = 20u;
                  i0 += i1;
                  l1 = i0;
                  i0 = l8;
                  i1 = 16u;
                  i0 += i1;
                  l4 = i0;
                  i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  i1 = l5;
                  i0 = i0 == i1;
                  if (i0) {
                    i0 = l4;
                  } else {
                    i0 = l1;
                  }
                  i1 = l0;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = l0;
                  i0 = !(i0);
                  if (i0) {
                    i0 = l3;
                    l0 = i0;
                    goto B65;
                  }
                }
                i0 = l0;
                i1 = l8;
                i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
                i0 = l5;
                i0 = i32_load(Z_envZ_memory, (u64)(i0 + 16));
                l1 = i0;
                if (i0) {
                  i0 = l0;
                  i1 = l1;
                  i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
                  i0 = l1;
                  i1 = l0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
                }
                i0 = l5;
                i0 = i32_load(Z_envZ_memory, (u64)(i0 + 20));
                l1 = i0;
                if (i0) {
                  i0 = l0;
                  i1 = l1;
                  i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
                  i0 = l1;
                  i1 = l0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
                }
              }
              i0 = l3;
              l0 = i0;
              B65:;
              i0 = l2;
              i1 = 16u;
              i0 = i0 < i1;
              if (i0) {
                i0 = l5;
                i1 = l2;
                i2 = p0;
                i1 += i2;
                p0 = i1;
                i2 = 3u;
                i1 |= i2;
                i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
                i0 = l5;
                i1 = p0;
                i0 += i1;
                i1 = 4u;
                i0 += i1;
                p0 = i0;
                i1 = p0;
                i1 = i32_load(Z_envZ_memory, (u64)(i1));
                i2 = 1u;
                i1 |= i2;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
              } else {
                i0 = l5;
                i1 = p0;
                i2 = 3u;
                i1 |= i2;
                i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
                i0 = l6;
                i1 = l2;
                i2 = 1u;
                i1 |= i2;
                i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
                i0 = l6;
                i1 = l2;
                i0 += i1;
                i1 = l2;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
                i0 = l2;
                i1 = 3u;
                i0 >>= (i1 & 31);
                l1 = i0;
                i0 = l2;
                i1 = 256u;
                i0 = i0 < i1;
                if (i0) {
                  i0 = l1;
                  i1 = 3u;
                  i0 <<= (i1 & 31);
                  i1 = 1068u;
                  i0 += i1;
                  p0 = i0;
                  i0 = 1028u;
                  i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  l0 = i0;
                  i1 = 1u;
                  i2 = l1;
                  i1 <<= (i2 & 31);
                  l1 = i1;
                  i0 &= i1;
                  if (i0) {
                    i0 = p0;
                    i1 = 8u;
                    i0 += i1;
                    l1 = i0;
                    i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  } else {
                    i0 = 1028u;
                    i1 = l0;
                    i2 = l1;
                    i1 |= i2;
                    i32_store(Z_envZ_memory, (u64)(i0), i1);
                    i0 = p0;
                    i1 = 8u;
                    i0 += i1;
                    l1 = i0;
                    i0 = p0;
                  }
                  l0 = i0;
                  i0 = l1;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = l0;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                  i0 = l6;
                  i1 = l0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
                  i0 = l6;
                  i1 = p0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                  goto B73;
                }
                i0 = l2;
                i1 = 8u;
                i0 >>= (i1 & 31);
                p0 = i0;
                if (i0) {
                  i0 = l2;
                  i1 = 16777215u;
                  i0 = i0 > i1;
                  if (i0) {
                    i0 = 31u;
                  } else {
                    i0 = l2;
                    i1 = 14u;
                    i2 = p0;
                    i3 = p0;
                    i4 = 1048320u;
                    i3 += i4;
                    i4 = 16u;
                    i3 >>= (i4 & 31);
                    i4 = 8u;
                    i3 &= i4;
                    p0 = i3;
                    i2 <<= (i3 & 31);
                    l1 = i2;
                    i3 = 520192u;
                    i2 += i3;
                    i3 = 16u;
                    i2 >>= (i3 & 31);
                    i3 = 4u;
                    i2 &= i3;
                    l3 = i2;
                    i3 = p0;
                    i2 |= i3;
                    i3 = l1;
                    i4 = l3;
                    i3 <<= (i4 & 31);
                    p0 = i3;
                    i4 = 245760u;
                    i3 += i4;
                    i4 = 16u;
                    i3 >>= (i4 & 31);
                    i4 = 2u;
                    i3 &= i4;
                    l1 = i3;
                    i2 |= i3;
                    i1 -= i2;
                    i2 = p0;
                    i3 = l1;
                    i2 <<= (i3 & 31);
                    i3 = 15u;
                    i2 >>= (i3 & 31);
                    i1 += i2;
                    p0 = i1;
                    i2 = 7u;
                    i1 += i2;
                    i0 >>= (i1 & 31);
                    i1 = 1u;
                    i0 &= i1;
                    i1 = p0;
                    i2 = 1u;
                    i1 <<= (i2 & 31);
                    i0 |= i1;
                  }
                } else {
                  i0 = 0u;
                }
                l1 = i0;
                i1 = 2u;
                i0 <<= (i1 & 31);
                i1 = 1332u;
                i0 += i1;
                p0 = i0;
                i0 = l6;
                i1 = l1;
                i32_store(Z_envZ_memory, (u64)(i0 + 28), i1);
                i0 = l6;
                i1 = 16u;
                i0 += i1;
                l3 = i0;
                i1 = 0u;
                i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
                i0 = l3;
                i1 = 0u;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
                i0 = l0;
                i1 = 1u;
                i2 = l1;
                i1 <<= (i2 & 31);
                l3 = i1;
                i0 &= i1;
                i0 = !(i0);
                if (i0) {
                  i0 = 1032u;
                  i1 = l0;
                  i2 = l3;
                  i1 |= i2;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = p0;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = l6;
                  i1 = p0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
                  i0 = l6;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                  i0 = l6;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
                  goto B73;
                }
                i0 = p0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                p0 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
                i1 = 4294967288u;
                i0 &= i1;
                i1 = l2;
                i0 = i0 == i1;
                if (i0) {
                  i0 = p0;
                } else {
                  i0 = 25u;
                  i1 = l1;
                  i2 = 1u;
                  i1 >>= (i2 & 31);
                  i0 -= i1;
                  l0 = i0;
                  i0 = l2;
                  i1 = l1;
                  i2 = 31u;
                  i1 = i1 == i2;
                  if (i1) {
                    i1 = 0u;
                  } else {
                    i1 = l0;
                  }
                  i0 <<= (i1 & 31);
                  l1 = i0;
                  L83: 
                    i0 = p0;
                    i1 = 16u;
                    i0 += i1;
                    i1 = l1;
                    i2 = 31u;
                    i1 >>= (i2 & 31);
                    i2 = 2u;
                    i1 <<= (i2 & 31);
                    i0 += i1;
                    l3 = i0;
                    i0 = i32_load(Z_envZ_memory, (u64)(i0));
                    l0 = i0;
                    if (i0) {
                      i0 = l1;
                      i1 = 1u;
                      i0 <<= (i1 & 31);
                      l1 = i0;
                      i0 = l0;
                      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
                      i1 = 4294967288u;
                      i0 &= i1;
                      i1 = l2;
                      i0 = i0 == i1;
                      if (i0) {goto B80;}
                      i0 = l0;
                      p0 = i0;
                      goto L83;
                    }
                  i0 = l3;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0), i1);
                  i0 = l6;
                  i1 = p0;
                  i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
                  i0 = l6;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                  i0 = l6;
                  i1 = l6;
                  i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
                  goto B73;
                }
                l0 = i0;
                B80:;
                i0 = l0;
                i1 = 8u;
                i0 += i1;
                p0 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                l1 = i0;
                i1 = l6;
                i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                i0 = p0;
                i1 = l6;
                i32_store(Z_envZ_memory, (u64)(i0), i1);
                i0 = l6;
                i1 = l1;
                i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
                i0 = l6;
                i1 = l0;
                i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
                i0 = l6;
                i1 = 0u;
                i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
              }
              B73:;
              i0 = l9;
              g4 = i0;
              i0 = l5;
              i1 = 8u;
              i0 += i1;
              goto Bfunc;
            }
          }
        }
      }
    }
  }
  B3:;
  i0 = 1036u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l1 = i0;
  i1 = p0;
  i0 = i0 >= i1;
  if (i0) {
    i0 = 1048u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l0 = i0;
    i0 = l1;
    i1 = p0;
    i0 -= i1;
    l2 = i0;
    i1 = 15u;
    i0 = i0 > i1;
    if (i0) {
      i0 = 1048u;
      i1 = l0;
      i2 = p0;
      i1 += i2;
      l3 = i1;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1036u;
      i1 = l2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l3;
      i1 = l2;
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l0;
      i1 = l1;
      i0 += i1;
      i1 = l2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l0;
      i1 = p0;
      i2 = 3u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    } else {
      i0 = 1036u;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1048u;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l0;
      i1 = l1;
      i2 = 3u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l0;
      i1 = l1;
      i0 += i1;
      i1 = 4u;
      i0 += i1;
      p0 = i0;
      i1 = p0;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
    }
    goto B1;
  }
  i0 = 1040u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l1 = i0;
  i1 = p0;
  i0 = i0 > i1;
  if (i0) {
    i0 = 1040u;
    i1 = l1;
    i2 = p0;
    i1 -= i2;
    l1 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    goto B2;
  }
  i0 = 1500u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  if (i0) {
    i0 = 1508u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
  } else {
    i0 = 1508u;
    i1 = 4096u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1504u;
    i1 = 4096u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1512u;
    i1 = 4294967295u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1516u;
    i1 = 4294967295u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1520u;
    i1 = 0u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1472u;
    i1 = 0u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1500u;
    i1 = l9;
    i2 = 4294967280u;
    i1 &= i2;
    i2 = 1431655768u;
    i1 ^= i2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 4096u;
  }
  l0 = i0;
  i1 = p0;
  i2 = 47u;
  i1 += i2;
  l5 = i1;
  i0 += i1;
  l4 = i0;
  i1 = 0u;
  i2 = l0;
  i1 -= i2;
  l7 = i1;
  i0 &= i1;
  l3 = i0;
  i1 = p0;
  i0 = i0 <= i1;
  if (i0) {
    goto B0;
  }
  i0 = 1468u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l0 = i0;
  if (i0) {
    i0 = 1460u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l2 = i0;
    i1 = l3;
    i0 += i1;
    l6 = i0;
    i1 = l2;
    i0 = i0 <= i1;
    i1 = l6;
    i2 = l0;
    i1 = i1 > i2;
    i0 |= i1;
    if (i0) {
      goto B0;
    }
  }
  i0 = p0;
  i1 = 48u;
  i0 += i1;
  l6 = i0;
  i0 = 1472u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  i1 = 4u;
  i0 &= i1;
  if (i0) {
    i0 = 0u;
    l1 = i0;
  } else {
    i0 = 1052u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l0 = i0;
    i0 = !(i0);
    if (i0) {goto B97;}
    i0 = 1476u;
    l2 = i0;
    L98: 
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l8 = i0;
      i1 = l0;
      i0 = i0 <= i1;
      if (i0) {
        i0 = l8;
        i1 = l2;
        i1 = i32_load(Z_envZ_memory, (u64)(i1 + 4));
        i0 += i1;
        i1 = l0;
        i0 = i0 > i1;
        if (i0) {goto B99;}
      }
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
      l2 = i0;
      if (i0) {goto L98;}
      goto B97;
      B99:;
    i0 = l4;
    i1 = l1;
    i0 -= i1;
    i1 = l7;
    i0 &= i1;
    l1 = i0;
    i1 = 2147483647u;
    i0 = i0 < i1;
    if (i0) {
      i0 = l1;
      i0 = f5(i0);
      l0 = i0;
      i1 = l2;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = l2;
      i2 = i32_load(Z_envZ_memory, (u64)(i2 + 4));
      i1 += i2;
      i0 = i0 == i1;
      if (i0) {
        i0 = l0;
        i1 = 4294967295u;
        i0 = i0 != i1;
        if (i0) {goto B93;}
      } else {
        goto B96;
      }
    } else {
      i0 = 0u;
      l1 = i0;
    }
    goto B95;
    B97:;
    i0 = 0u;
    i0 = f5(i0);
    l0 = i0;
    i1 = 4294967295u;
    i0 = i0 == i1;
    if (i0) {
      i0 = 0u;
    } else {
      i0 = 1504u;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l1 = i0;
      i1 = 4294967295u;
      i0 += i1;
      l2 = i0;
      i1 = l0;
      i0 += i1;
      i1 = 0u;
      i2 = l1;
      i1 -= i2;
      i0 &= i1;
      i1 = l0;
      i0 -= i1;
      l1 = i0;
      i0 = l2;
      i1 = l0;
      i0 &= i1;
      if (i0) {
        i0 = l1;
      } else {
        i0 = 0u;
      }
      i1 = l3;
      i0 += i1;
      l1 = i0;
      i1 = 1460u;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      l4 = i1;
      i0 += i1;
      l2 = i0;
      i0 = l1;
      i1 = p0;
      i0 = i0 > i1;
      i1 = l1;
      i2 = 2147483647u;
      i1 = i1 < i2;
      i0 &= i1;
      if (i0) {
        i0 = 1468u;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l7 = i0;
        if (i0) {
          i0 = l2;
          i1 = l4;
          i0 = i0 <= i1;
          i1 = l2;
          i2 = l7;
          i1 = i1 > i2;
          i0 |= i1;
          if (i0) {
            i0 = 0u;
            l1 = i0;
            goto B95;
          }
        }
        i0 = l1;
        i0 = f5(i0);
        l2 = i0;
        i1 = l0;
        i0 = i0 == i1;
        if (i0) {goto B93;}
        i0 = l2;
        l0 = i0;
        goto B96;
      } else {
        i0 = 0u;
      }
    }
    l1 = i0;
    goto B95;
    B96:;
    i0 = l6;
    i1 = l1;
    i0 = i0 > i1;
    i1 = l1;
    i2 = 2147483647u;
    i1 = i1 < i2;
    i2 = l0;
    i3 = 4294967295u;
    i2 = i2 != i3;
    i1 &= i2;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {
      i0 = l0;
      i1 = 4294967295u;
      i0 = i0 == i1;
      if (i0) {
        i0 = 0u;
        l1 = i0;
        goto B95;
      } else {
        goto B93;
      }
      UNREACHABLE;
    }
    i0 = l5;
    i1 = l1;
    i0 -= i1;
    i1 = 1508u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    l2 = i1;
    i0 += i1;
    i1 = 0u;
    i2 = l2;
    i1 -= i2;
    i0 &= i1;
    l2 = i0;
    i1 = 2147483647u;
    i0 = i0 >= i1;
    if (i0) {goto B93;}
    i0 = 0u;
    i1 = l1;
    i0 -= i1;
    l5 = i0;
    i0 = l2;
    i0 = f5(i0);
    i1 = 4294967295u;
    i0 = i0 == i1;
    if (i0) {
      i0 = l5;
      i0 = f5(i0);
      i0 = 0u;
    } else {
      i0 = l2;
      i1 = l1;
      i0 += i1;
      l1 = i0;
      goto B93;
    }
    l1 = i0;
    B95:;
    i0 = 1472u;
    i1 = 1472u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i2 = 4u;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
  }
  i0 = l3;
  i1 = 2147483647u;
  i0 = i0 < i1;
  if (i0) {
    i0 = l3;
    i0 = f5(i0);
    l0 = i0;
    i1 = 0u;
    i1 = f5(i1);
    l2 = i1;
    i0 = i0 < i1;
    i1 = l0;
    i2 = 4294967295u;
    i1 = i1 != i2;
    i2 = l2;
    i3 = 4294967295u;
    i2 = i2 != i3;
    i1 &= i2;
    i0 &= i1;
    l3 = i0;
    i0 = l2;
    i1 = l0;
    i0 -= i1;
    l2 = i0;
    i1 = p0;
    i2 = 40u;
    i1 += i2;
    i0 = i0 > i1;
    l5 = i0;
    if (i0) {
      i0 = l2;
      l1 = i0;
    }
    i0 = l0;
    i1 = 4294967295u;
    i0 = i0 == i1;
    i1 = l5;
    i2 = 1u;
    i1 ^= i2;
    i0 |= i1;
    i1 = l3;
    i2 = 1u;
    i1 ^= i2;
    i0 |= i1;
    i0 = !(i0);
    if (i0) {goto B93;}
  }
  goto B92;
  B93:;
  i0 = 1460u;
  i1 = 1460u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i2 = l1;
  i1 += i2;
  l2 = i1;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  i0 = l2;
  i1 = 1464u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i0 = i0 > i1;
  if (i0) {
    i0 = 1464u;
    i1 = l2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
  }
  i0 = 1052u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l3 = i0;
  if (i0) {
    i0 = 1476u;
    l2 = i0;
    L118: 
      i0 = l0;
      i1 = l2;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      l5 = i1;
      i2 = l2;
      i2 = i32_load(Z_envZ_memory, (u64)(i2 + 4));
      l4 = i2;
      i1 += i2;
      i0 = i0 == i1;
      if (i0) {goto B117;}
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
      l2 = i0;
      if (i0) {goto L118;}
    goto B116;
    B117:;
    i0 = l2;
    i1 = 4u;
    i0 += i1;
    l7 = i0;
    i0 = l2;
    i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
    i1 = 8u;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {
      i0 = l0;
      i1 = l3;
      i0 = i0 > i1;
      i1 = l5;
      i2 = l3;
      i1 = i1 <= i2;
      i0 &= i1;
      if (i0) {
        i0 = l7;
        i1 = l4;
        i2 = l1;
        i1 += i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = 1040u;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        i1 = l1;
        i0 += i1;
        l1 = i0;
        i0 = 0u;
        i1 = l3;
        i2 = 8u;
        i1 += i2;
        l2 = i1;
        i0 -= i1;
        i1 = 7u;
        i0 &= i1;
        l0 = i0;
        i0 = 1052u;
        i1 = l3;
        i2 = l2;
        i3 = 7u;
        i2 &= i3;
        if (i2) {
          i2 = l0;
        } else {
          i2 = 0u;
          l0 = i2;
        }
        i1 += i2;
        l2 = i1;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = 1040u;
        i1 = l1;
        i2 = l0;
        i1 -= i2;
        l0 = i1;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l2;
        i1 = l0;
        i2 = 1u;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = l3;
        i1 = l1;
        i0 += i1;
        i1 = 40u;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = 1056u;
        i1 = 1516u;
        i1 = i32_load(Z_envZ_memory, (u64)(i1));
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        goto B114;
      }
    }
    B116:;
    i0 = l0;
    i1 = 1044u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i0 = i0 < i1;
    if (i0) {
      i0 = 1044u;
      i1 = l0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
    }
    i0 = l0;
    i1 = l1;
    i0 += i1;
    l5 = i0;
    i0 = 1476u;
    l2 = i0;
    L125: 
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      i1 = l5;
      i0 = i0 == i1;
      if (i0) {goto B124;}
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
      l2 = i0;
      if (i0) {goto L125;}
    goto B123;
    B124:;
    i0 = l2;
    i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
    i1 = 8u;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {
      i0 = l2;
      i1 = l0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l2;
      i1 = 4u;
      i0 += i1;
      l2 = i0;
      i1 = l2;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = l1;
      i1 += i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 0u;
      i1 = l0;
      i2 = 8u;
      i1 += i2;
      l1 = i1;
      i0 -= i1;
      i1 = 7u;
      i0 &= i1;
      l2 = i0;
      i0 = 0u;
      i1 = l5;
      i2 = 8u;
      i1 += i2;
      l7 = i1;
      i0 -= i1;
      i1 = 7u;
      i0 &= i1;
      l8 = i0;
      i0 = l0;
      i1 = l1;
      i2 = 7u;
      i1 &= i2;
      if (i1) {
        i1 = l2;
      } else {
        i1 = 0u;
      }
      i0 += i1;
      l6 = i0;
      i1 = p0;
      i0 += i1;
      l4 = i0;
      i0 = l5;
      i1 = l7;
      i2 = 7u;
      i1 &= i2;
      if (i1) {
        i1 = l8;
      } else {
        i1 = 0u;
      }
      i0 += i1;
      l1 = i0;
      i1 = l6;
      i0 -= i1;
      i1 = p0;
      i0 -= i1;
      l2 = i0;
      i0 = l6;
      i1 = p0;
      i2 = 3u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l3;
      i1 = l1;
      i0 = i0 == i1;
      if (i0) {
        i0 = 1040u;
        i1 = 1040u;
        i1 = i32_load(Z_envZ_memory, (u64)(i1));
        i2 = l2;
        i1 += i2;
        p0 = i1;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = 1052u;
        i1 = l4;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l4;
        i1 = p0;
        i2 = 1u;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      } else {
        i0 = 1048u;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        i1 = l1;
        i0 = i0 == i1;
        if (i0) {
          i0 = 1036u;
          i1 = 1036u;
          i1 = i32_load(Z_envZ_memory, (u64)(i1));
          i2 = l2;
          i1 += i2;
          p0 = i1;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = 1048u;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l4;
          i1 = p0;
          i2 = 1u;
          i1 |= i2;
          i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
          i0 = l4;
          i1 = p0;
          i0 += i1;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          goto B129;
        }
        i0 = l1;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
        p0 = i0;
        i1 = 3u;
        i0 &= i1;
        i1 = 1u;
        i0 = i0 == i1;
        if (i0) {
          i0 = p0;
          i1 = 4294967288u;
          i0 &= i1;
          l8 = i0;
          i0 = p0;
          i1 = 3u;
          i0 >>= (i1 & 31);
          l3 = i0;
          i0 = p0;
          i1 = 256u;
          i0 = i0 < i1;
          if (i0) {
            i0 = l1;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
            p0 = i0;
            i1 = l1;
            i1 = i32_load(Z_envZ_memory, (u64)(i1 + 8));
            l0 = i1;
            i0 = i0 == i1;
            if (i0) {
              i0 = 1028u;
              i1 = 1028u;
              i1 = i32_load(Z_envZ_memory, (u64)(i1));
              i2 = 1u;
              i3 = l3;
              i2 <<= (i3 & 31);
              i3 = 4294967295u;
              i2 ^= i3;
              i1 &= i2;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
            } else {
              i0 = l0;
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
              i0 = p0;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
            }
          } else {
            i0 = l1;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 24));
            l7 = i0;
            i0 = l1;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
            p0 = i0;
            i1 = l1;
            i0 = i0 == i1;
            if (i0) {
              i0 = l1;
              i1 = 16u;
              i0 += i1;
              l0 = i0;
              i1 = 4u;
              i0 += i1;
              l3 = i0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              p0 = i0;
              if (i0) {
                i0 = l3;
                l0 = i0;
              } else {
                i0 = l0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                p0 = i0;
                i0 = !(i0);
                if (i0) {
                  i0 = 0u;
                  p0 = i0;
                  goto B136;
                }
              }
              L140: 
                i0 = p0;
                i1 = 20u;
                i0 += i1;
                l3 = i0;
                i0 = i32_load(Z_envZ_memory, (u64)(i0));
                l5 = i0;
                i0 = !(i0);
                if (i0) {
                  i0 = p0;
                  i1 = 16u;
                  i0 += i1;
                  l3 = i0;
                  i0 = i32_load(Z_envZ_memory, (u64)(i0));
                  l5 = i0;
                  i0 = !(i0);
                  if (i0) {goto B141;}
                }
                i0 = l3;
                l0 = i0;
                i0 = l5;
                p0 = i0;
                goto L140;
                B141:;
              i0 = l0;
              i1 = 0u;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
            } else {
              i0 = l1;
              i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
              l0 = i0;
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
              i0 = p0;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
            }
            B136:;
            i0 = l7;
            i0 = !(i0);
            if (i0) {goto B133;}
            i0 = l1;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 28));
            l0 = i0;
            i1 = 2u;
            i0 <<= (i1 & 31);
            i1 = 1332u;
            i0 += i1;
            l3 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
            i1 = l1;
            i0 = i0 == i1;
            if (i0) {
              i0 = l3;
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              i0 = p0;
              if (i0) {goto B143;}
              i0 = 1032u;
              i1 = 1032u;
              i1 = i32_load(Z_envZ_memory, (u64)(i1));
              i2 = 1u;
              i3 = l0;
              i2 <<= (i3 & 31);
              i3 = 4294967295u;
              i2 ^= i3;
              i1 &= i2;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              goto B133;
            } else {
              i0 = l7;
              i1 = 20u;
              i0 += i1;
              l0 = i0;
              i0 = l7;
              i1 = 16u;
              i0 += i1;
              l3 = i0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0));
              i1 = l1;
              i0 = i0 == i1;
              if (i0) {
                i0 = l3;
              } else {
                i0 = l0;
              }
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0), i1);
              i0 = p0;
              i0 = !(i0);
              if (i0) {goto B133;}
            }
            B143:;
            i0 = p0;
            i1 = l7;
            i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
            i0 = l1;
            i1 = 16u;
            i0 += i1;
            l3 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
            l0 = i0;
            if (i0) {
              i0 = p0;
              i1 = l0;
              i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
              i0 = l0;
              i1 = p0;
              i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
            }
            i0 = l3;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
            l0 = i0;
            i0 = !(i0);
            if (i0) {goto B133;}
            i0 = p0;
            i1 = l0;
            i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
            i0 = l0;
            i1 = p0;
            i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
          }
          B133:;
          i0 = l1;
          i1 = l8;
          i0 += i1;
          l1 = i0;
          i0 = l8;
          i1 = l2;
          i0 += i1;
          l2 = i0;
        }
        i0 = l1;
        i1 = 4u;
        i0 += i1;
        p0 = i0;
        i1 = p0;
        i1 = i32_load(Z_envZ_memory, (u64)(i1));
        i2 = 4294967294u;
        i1 &= i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l4;
        i1 = l2;
        i2 = 1u;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = l4;
        i1 = l2;
        i0 += i1;
        i1 = l2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l2;
        i1 = 3u;
        i0 >>= (i1 & 31);
        l0 = i0;
        i0 = l2;
        i1 = 256u;
        i0 = i0 < i1;
        if (i0) {
          i0 = l0;
          i1 = 3u;
          i0 <<= (i1 & 31);
          i1 = 1068u;
          i0 += i1;
          p0 = i0;
          i0 = 1028u;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          l1 = i0;
          i1 = 1u;
          i2 = l0;
          i1 <<= (i2 & 31);
          l0 = i1;
          i0 &= i1;
          if (i0) {
            i0 = p0;
            i1 = 8u;
            i0 += i1;
            l1 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
          } else {
            i0 = 1028u;
            i1 = l1;
            i2 = l0;
            i1 |= i2;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
            i0 = p0;
            i1 = 8u;
            i0 += i1;
            l1 = i0;
            i0 = p0;
          }
          l0 = i0;
          i0 = l1;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l0;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          i0 = l4;
          i1 = l0;
          i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
          i0 = l4;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          goto B129;
        }
        i0 = l2;
        i1 = 8u;
        i0 >>= (i1 & 31);
        p0 = i0;
        if (i0) {
          i0 = 31u;
          i1 = l2;
          i2 = 16777215u;
          i1 = i1 > i2;
          if (i1) {goto B149;}
          i0 = l2;
          i1 = 14u;
          i2 = p0;
          i3 = p0;
          i4 = 1048320u;
          i3 += i4;
          i4 = 16u;
          i3 >>= (i4 & 31);
          i4 = 8u;
          i3 &= i4;
          p0 = i3;
          i2 <<= (i3 & 31);
          l0 = i2;
          i3 = 520192u;
          i2 += i3;
          i3 = 16u;
          i2 >>= (i3 & 31);
          i3 = 4u;
          i2 &= i3;
          l1 = i2;
          i3 = p0;
          i2 |= i3;
          i3 = l0;
          i4 = l1;
          i3 <<= (i4 & 31);
          p0 = i3;
          i4 = 245760u;
          i3 += i4;
          i4 = 16u;
          i3 >>= (i4 & 31);
          i4 = 2u;
          i3 &= i4;
          l0 = i3;
          i2 |= i3;
          i1 -= i2;
          i2 = p0;
          i3 = l0;
          i2 <<= (i3 & 31);
          i3 = 15u;
          i2 >>= (i3 & 31);
          i1 += i2;
          p0 = i1;
          i2 = 7u;
          i1 += i2;
          i0 >>= (i1 & 31);
          i1 = 1u;
          i0 &= i1;
          i1 = p0;
          i2 = 1u;
          i1 <<= (i2 & 31);
          i0 |= i1;
        } else {
          i0 = 0u;
        }
        B149:;
        l0 = i0;
        i1 = 2u;
        i0 <<= (i1 & 31);
        i1 = 1332u;
        i0 += i1;
        p0 = i0;
        i0 = l4;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 28), i1);
        i0 = l4;
        i1 = 16u;
        i0 += i1;
        l1 = i0;
        i1 = 0u;
        i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
        i0 = l1;
        i1 = 0u;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = 1032u;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l1 = i0;
        i1 = 1u;
        i2 = l0;
        i1 <<= (i2 & 31);
        l3 = i1;
        i0 &= i1;
        i0 = !(i0);
        if (i0) {
          i0 = 1032u;
          i1 = l1;
          i2 = l3;
          i1 |= i2;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = p0;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l4;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
          i0 = l4;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          i0 = l4;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
          goto B129;
        }
        i0 = p0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        p0 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
        i1 = 4294967288u;
        i0 &= i1;
        i1 = l2;
        i0 = i0 == i1;
        if (i0) {
          i0 = p0;
        } else {
          i0 = 25u;
          i1 = l0;
          i2 = 1u;
          i1 >>= (i2 & 31);
          i0 -= i1;
          l1 = i0;
          i0 = l2;
          i1 = l0;
          i2 = 31u;
          i1 = i1 == i2;
          if (i1) {
            i1 = 0u;
          } else {
            i1 = l1;
          }
          i0 <<= (i1 & 31);
          l1 = i0;
          L155: 
            i0 = p0;
            i1 = 16u;
            i0 += i1;
            i1 = l1;
            i2 = 31u;
            i1 >>= (i2 & 31);
            i2 = 2u;
            i1 <<= (i2 & 31);
            i0 += i1;
            l3 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
            l0 = i0;
            if (i0) {
              i0 = l1;
              i1 = 1u;
              i0 <<= (i1 & 31);
              l1 = i0;
              i0 = l0;
              i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
              i1 = 4294967288u;
              i0 &= i1;
              i1 = l2;
              i0 = i0 == i1;
              if (i0) {goto B152;}
              i0 = l0;
              p0 = i0;
              goto L155;
            }
          i0 = l3;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l4;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
          i0 = l4;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
          i0 = l4;
          i1 = l4;
          i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
          goto B129;
        }
        l0 = i0;
        B152:;
        i0 = l0;
        i1 = 8u;
        i0 += i1;
        p0 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l1 = i0;
        i1 = l4;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = p0;
        i1 = l4;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l4;
        i1 = l1;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
        i0 = l4;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l4;
        i1 = 0u;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
      }
      B129:;
      i0 = l9;
      g4 = i0;
      i0 = l6;
      i1 = 8u;
      i0 += i1;
      goto Bfunc;
    }
    B123:;
    i0 = 1476u;
    l2 = i0;
    L157: 
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l5 = i0;
      i1 = l3;
      i0 = i0 <= i1;
      if (i0) {
        i0 = l5;
        i1 = l2;
        i1 = i32_load(Z_envZ_memory, (u64)(i1 + 4));
        i0 += i1;
        l6 = i0;
        i1 = l3;
        i0 = i0 > i1;
        if (i0) {goto B158;}
      }
      i0 = l2;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
      l2 = i0;
      goto L157;
      B158:;
    i0 = 0u;
    i1 = l6;
    i2 = 4294967249u;
    i1 += i2;
    l2 = i1;
    i2 = 8u;
    i1 += i2;
    l5 = i1;
    i0 -= i1;
    i1 = 7u;
    i0 &= i1;
    l4 = i0;
    i0 = l2;
    i1 = l5;
    i2 = 7u;
    i1 &= i2;
    if (i1) {
      i1 = l4;
    } else {
      i1 = 0u;
    }
    i0 += i1;
    l2 = i0;
    i1 = l3;
    i2 = 16u;
    i1 += i2;
    l11 = i1;
    i0 = i0 < i1;
    if (i0) {
      i0 = l3;
      l2 = i0;
    } else {
      i0 = l2;
    }
    i1 = 8u;
    i0 += i1;
    l7 = i0;
    i0 = l2;
    i1 = 24u;
    i0 += i1;
    l5 = i0;
    i0 = l1;
    i1 = 4294967256u;
    i0 += i1;
    l8 = i0;
    i0 = 0u;
    i1 = l0;
    i2 = 8u;
    i1 += i2;
    l10 = i1;
    i0 -= i1;
    i1 = 7u;
    i0 &= i1;
    l4 = i0;
    i0 = 1052u;
    i1 = l0;
    i2 = l10;
    i3 = 7u;
    i2 &= i3;
    if (i2) {
      i2 = l4;
    } else {
      i2 = 0u;
      l4 = i2;
    }
    i1 += i2;
    l10 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1040u;
    i1 = l8;
    i2 = l4;
    i1 -= i2;
    l4 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l10;
    i1 = l4;
    i2 = 1u;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = l0;
    i1 = l8;
    i0 += i1;
    i1 = 40u;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = 1056u;
    i1 = 1516u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l2;
    i1 = 4u;
    i0 += i1;
    l4 = i0;
    i1 = 27u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l7;
    i1 = 1476u;
    j1 = i64_load(Z_envZ_memory, (u64)(i1));
    i64_store(Z_envZ_memory, (u64)(i0), j1);
    i0 = l7;
    i1 = 1484u;
    j1 = i64_load(Z_envZ_memory, (u64)(i1));
    i64_store(Z_envZ_memory, (u64)(i0 + 8), j1);
    i0 = 1476u;
    i1 = l0;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1480u;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1488u;
    i1 = 0u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1484u;
    i1 = l7;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l5;
    l0 = i0;
    L163: 
      i0 = l0;
      i1 = 4u;
      i0 += i1;
      l1 = i0;
      i1 = 7u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l0;
      i1 = 8u;
      i0 += i1;
      i1 = l6;
      i0 = i0 < i1;
      if (i0) {
        i0 = l1;
        l0 = i0;
        goto L163;
      }
    i0 = l2;
    i1 = l3;
    i0 = i0 != i1;
    if (i0) {
      i0 = l4;
      i1 = l4;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = 4294967294u;
      i1 &= i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l3;
      i1 = l2;
      i2 = l3;
      i1 -= i2;
      l5 = i1;
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l2;
      i1 = l5;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l5;
      i1 = 3u;
      i0 >>= (i1 & 31);
      l1 = i0;
      i0 = l5;
      i1 = 256u;
      i0 = i0 < i1;
      if (i0) {
        i0 = l1;
        i1 = 3u;
        i0 <<= (i1 & 31);
        i1 = 1068u;
        i0 += i1;
        l0 = i0;
        i0 = 1028u;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l2 = i0;
        i1 = 1u;
        i2 = l1;
        i1 <<= (i2 & 31);
        l1 = i1;
        i0 &= i1;
        if (i0) {
          i0 = l0;
          i1 = 8u;
          i0 += i1;
          l2 = i0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
        } else {
          i0 = 1028u;
          i1 = l2;
          i2 = l1;
          i1 |= i2;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l0;
          i1 = 8u;
          i0 += i1;
          l2 = i0;
          i0 = l0;
        }
        l1 = i0;
        i0 = l2;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l1;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l3;
        i1 = l1;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
        i0 = l3;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        goto B114;
      }
      i0 = l5;
      i1 = 8u;
      i0 >>= (i1 & 31);
      l0 = i0;
      if (i0) {
        i0 = l5;
        i1 = 16777215u;
        i0 = i0 > i1;
        if (i0) {
          i0 = 31u;
        } else {
          i0 = l5;
          i1 = 14u;
          i2 = l0;
          i3 = l0;
          i4 = 1048320u;
          i3 += i4;
          i4 = 16u;
          i3 >>= (i4 & 31);
          i4 = 8u;
          i3 &= i4;
          l0 = i3;
          i2 <<= (i3 & 31);
          l1 = i2;
          i3 = 520192u;
          i2 += i3;
          i3 = 16u;
          i2 >>= (i3 & 31);
          i3 = 4u;
          i2 &= i3;
          l2 = i2;
          i3 = l0;
          i2 |= i3;
          i3 = l1;
          i4 = l2;
          i3 <<= (i4 & 31);
          l0 = i3;
          i4 = 245760u;
          i3 += i4;
          i4 = 16u;
          i3 >>= (i4 & 31);
          i4 = 2u;
          i3 &= i4;
          l1 = i3;
          i2 |= i3;
          i1 -= i2;
          i2 = l0;
          i3 = l1;
          i2 <<= (i3 & 31);
          i3 = 15u;
          i2 >>= (i3 & 31);
          i1 += i2;
          l0 = i1;
          i2 = 7u;
          i1 += i2;
          i0 >>= (i1 & 31);
          i1 = 1u;
          i0 &= i1;
          i1 = l0;
          i2 = 1u;
          i1 <<= (i2 & 31);
          i0 |= i1;
        }
      } else {
        i0 = 0u;
      }
      l1 = i0;
      i1 = 2u;
      i0 <<= (i1 & 31);
      i1 = 1332u;
      i0 += i1;
      l0 = i0;
      i0 = l3;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0 + 28), i1);
      i0 = l3;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
      i0 = l11;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1032u;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l2 = i0;
      i1 = 1u;
      i2 = l1;
      i1 <<= (i2 & 31);
      l4 = i1;
      i0 &= i1;
      i0 = !(i0);
      if (i0) {
        i0 = 1032u;
        i1 = l2;
        i2 = l4;
        i1 |= i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l0;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l3;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
        i0 = l3;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l3;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
        goto B114;
      }
      i0 = l0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l0 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
      i1 = 4294967288u;
      i0 &= i1;
      i1 = l5;
      i0 = i0 == i1;
      if (i0) {
        i0 = l0;
      } else {
        i0 = 25u;
        i1 = l1;
        i2 = 1u;
        i1 >>= (i2 & 31);
        i0 -= i1;
        l2 = i0;
        i0 = l5;
        i1 = l1;
        i2 = 31u;
        i1 = i1 == i2;
        if (i1) {
          i1 = 0u;
        } else {
          i1 = l2;
        }
        i0 <<= (i1 & 31);
        l2 = i0;
        L174: 
          i0 = l0;
          i1 = 16u;
          i0 += i1;
          i1 = l2;
          i2 = 31u;
          i1 >>= (i2 & 31);
          i2 = 2u;
          i1 <<= (i2 & 31);
          i0 += i1;
          l4 = i0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          l1 = i0;
          if (i0) {
            i0 = l2;
            i1 = 1u;
            i0 <<= (i1 & 31);
            l2 = i0;
            i0 = l1;
            i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
            i1 = 4294967288u;
            i0 &= i1;
            i1 = l5;
            i0 = i0 == i1;
            if (i0) {goto B171;}
            i0 = l1;
            l0 = i0;
            goto L174;
          }
        i0 = l4;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l3;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
        i0 = l3;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l3;
        i1 = l3;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
        goto B114;
      }
      l1 = i0;
      B171:;
      i0 = l1;
      i1 = 8u;
      i0 += i1;
      l0 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l2 = i0;
      i1 = l3;
      i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
      i0 = l0;
      i1 = l3;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l3;
      i1 = l2;
      i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
      i0 = l3;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
      i0 = l3;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
    }
  } else {
    i0 = 1044u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l2 = i0;
    i0 = !(i0);
    i1 = l0;
    i2 = l2;
    i1 = i1 < i2;
    i0 |= i1;
    if (i0) {
      i0 = 1044u;
      i1 = l0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
    }
    i0 = 1476u;
    i1 = l0;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1480u;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1488u;
    i1 = 0u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1064u;
    i1 = 1500u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1060u;
    i1 = 4294967295u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1080u;
    i1 = 1068u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1076u;
    i1 = 1068u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1088u;
    i1 = 1076u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1084u;
    i1 = 1076u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1096u;
    i1 = 1084u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1092u;
    i1 = 1084u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1104u;
    i1 = 1092u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1100u;
    i1 = 1092u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1112u;
    i1 = 1100u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1108u;
    i1 = 1100u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1120u;
    i1 = 1108u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1116u;
    i1 = 1108u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1128u;
    i1 = 1116u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1124u;
    i1 = 1116u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1136u;
    i1 = 1124u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1132u;
    i1 = 1124u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1144u;
    i1 = 1132u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1140u;
    i1 = 1132u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1152u;
    i1 = 1140u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1148u;
    i1 = 1140u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1160u;
    i1 = 1148u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1156u;
    i1 = 1148u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1168u;
    i1 = 1156u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1164u;
    i1 = 1156u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1176u;
    i1 = 1164u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1172u;
    i1 = 1164u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1184u;
    i1 = 1172u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1180u;
    i1 = 1172u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1192u;
    i1 = 1180u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1188u;
    i1 = 1180u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1200u;
    i1 = 1188u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1196u;
    i1 = 1188u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1208u;
    i1 = 1196u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1204u;
    i1 = 1196u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1216u;
    i1 = 1204u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1212u;
    i1 = 1204u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1224u;
    i1 = 1212u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1220u;
    i1 = 1212u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1232u;
    i1 = 1220u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1228u;
    i1 = 1220u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1240u;
    i1 = 1228u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1236u;
    i1 = 1228u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1248u;
    i1 = 1236u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1244u;
    i1 = 1236u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1256u;
    i1 = 1244u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1252u;
    i1 = 1244u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1264u;
    i1 = 1252u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1260u;
    i1 = 1252u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1272u;
    i1 = 1260u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1268u;
    i1 = 1260u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1280u;
    i1 = 1268u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1276u;
    i1 = 1268u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1288u;
    i1 = 1276u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1284u;
    i1 = 1276u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1296u;
    i1 = 1284u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1292u;
    i1 = 1284u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1304u;
    i1 = 1292u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1300u;
    i1 = 1292u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1312u;
    i1 = 1300u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1308u;
    i1 = 1300u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1320u;
    i1 = 1308u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1316u;
    i1 = 1308u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1328u;
    i1 = 1316u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1324u;
    i1 = 1316u;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l1;
    i1 = 4294967256u;
    i0 += i1;
    l2 = i0;
    i0 = 0u;
    i1 = l0;
    i2 = 8u;
    i1 += i2;
    l3 = i1;
    i0 -= i1;
    i1 = 7u;
    i0 &= i1;
    l1 = i0;
    i0 = 1052u;
    i1 = l0;
    i2 = l3;
    i3 = 7u;
    i2 &= i3;
    if (i2) {
      i2 = l1;
    } else {
      i2 = 0u;
      l1 = i2;
    }
    i1 += i2;
    l3 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = 1040u;
    i1 = l2;
    i2 = l1;
    i1 -= i2;
    l1 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l3;
    i1 = l1;
    i2 = 1u;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = l0;
    i1 = l2;
    i0 += i1;
    i1 = 40u;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = 1056u;
    i1 = 1516u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i32_store(Z_envZ_memory, (u64)(i0), i1);
  }
  B114:;
  i0 = 1040u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l0 = i0;
  i1 = p0;
  i0 = i0 > i1;
  if (i0) {
    i0 = 1040u;
    i1 = l0;
    i2 = p0;
    i1 -= i2;
    l1 = i1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    goto B2;
  }
  B92:;
  i0 = 1524u;
  i1 = 12u;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  goto B0;
  B2:;
  i0 = 1052u;
  i1 = 1052u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  l0 = i1;
  i2 = p0;
  i1 += i2;
  l2 = i1;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  i0 = l2;
  i1 = l1;
  i2 = 1u;
  i1 |= i2;
  i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
  i0 = l0;
  i1 = p0;
  i2 = 3u;
  i1 |= i2;
  i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
  B1:;
  i0 = l9;
  g4 = i0;
  i0 = l0;
  i1 = 8u;
  i0 += i1;
  goto Bfunc;
  B0:;
  i0 = l9;
  g4 = i0;
  i0 = 0u;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static void _colorFromHexString(u32 p0, u32 p1) {
  u32 l0 = 0, l1 = 0, l2 = 0, l3 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3;
  f32 f1;
  f64 d1, d2;
  i0 = p0;
  i1 = 1u;
  i0 += i1;
  l0 = i0;
  i0 = p0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0));
  i1 = 35u;
  i0 = i0 != i1;
  if (i0) {
    i0 = p0;
    l0 = i0;
  }
  i0 = p1;
  f1 = 1;
  f32_store(Z_envZ_memory, (u64)(i0 + 12), f1);
  i0 = p1;
  i1 = 8u;
  i0 += i1;
  l1 = i0;
  f1 = 1;
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  i0 = p1;
  i1 = 4u;
  i0 += i1;
  l2 = i0;
  f1 = 1;
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  i0 = p1;
  f1 = 1;
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  i0 = l0;
  i0 = f10(i0);
  i1 = 6u;
  i0 = i0 != i1;
  if (i0) {
    goto Bfunc;
  }
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0 + 1));
  l3 = i0;
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0));
  i1 = 1024u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i2 = 1u;
  i1 &= i2;
  i0 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i1, i0);
  i1 = 4u;
  i0 <<= (i1 & 31);
  p0 = i0;
  i0 = p1;
  i1 = l3;
  i2 = 1024u;
  i2 = i32_load(Z_envZ_memory, (u64)(i2));
  i3 = 1u;
  i2 &= i3;
  i1 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i2, i1);
  i2 = p0;
  i1 += i2;
  d1 = (f64)(s32)(i1);
  d2 = 255;
  d1 /= d2;
  f1 = (f32)(d1);
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0 + 3));
  p1 = i0;
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0 + 2));
  i1 = 1024u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i2 = 1u;
  i1 &= i2;
  i0 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i1, i0);
  i1 = 4u;
  i0 <<= (i1 & 31);
  p0 = i0;
  i0 = l2;
  i1 = p1;
  i2 = 1024u;
  i2 = i32_load(Z_envZ_memory, (u64)(i2));
  i3 = 1u;
  i2 &= i3;
  i1 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i2, i1);
  i2 = p0;
  i1 += i2;
  d1 = (f64)(s32)(i1);
  d2 = 255;
  d1 /= d2;
  f1 = (f32)(d1);
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0 + 5));
  p1 = i0;
  i0 = l0;
  i0 = i32_load8_s(Z_envZ_memory, (u64)(i0 + 4));
  i1 = 1024u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i2 = 1u;
  i1 &= i2;
  i0 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i1, i0);
  i1 = 4u;
  i0 <<= (i1 & 31);
  p0 = i0;
  i0 = l1;
  i1 = p1;
  i2 = 1024u;
  i2 = i32_load(Z_envZ_memory, (u64)(i2));
  i3 = 1u;
  i2 &= i3;
  i1 = CALL_INDIRECT((*Z_envZ_table), u32 (*)(u32), 0, i2, i1);
  i2 = p0;
  i1 += i2;
  d1 = (f64)(s32)(i1);
  d2 = 255;
  d1 /= d2;
  f1 = (f32)(d1);
  f32_store(Z_envZ_memory, (u64)(i0), f1);
  Bfunc:;
  FUNC_EPILOGUE;
}

static u32 f8(u32 p0) {
  u32 l0 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = p0;
  i1 = 24u;
  i0 <<= (i1 & 31);
  i1 = 24u;
  i0 = (u32)((s32)i0 >> (i1 & 31));
  l0 = i0;
  i0 = p0;
  i1 = 4294967248u;
  i0 += i1;
  i1 = 24u;
  i0 <<= (i1 & 31);
  i1 = 24u;
  i0 = (u32)((s32)i0 >> (i1 & 31));
  i1 = 255u;
  i0 &= i1;
  i1 = 10u;
  i0 = (u32)((s32)i0 < (s32)i1);
  if (i0) {
    i0 = l0;
    i1 = 4294967248u;
    i0 += i1;
  } else {
    i0 = l0;
    i1 = 32u;
    i0 |= i1;
    p0 = i0;
    i0 = l0;
    i1 = 4294967231u;
    i0 += i1;
    i1 = 26u;
    i0 = i0 < i1;
    if (i0) {
      i0 = p0;
    } else {
      i0 = l0;
    }
    i1 = 24u;
    i0 <<= (i1 & 31);
    p0 = i0;
    i1 = 24u;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    i1 = 4294967209u;
    i0 += i1;
    l0 = i0;
    i0 = p0;
    i1 = 2684354559u;
    i0 += i1;
    i1 = 117440511u;
    i0 = i0 < i1;
    if (i0) {
      i0 = l0;
    } else {
      i0 = 4294967295u;
    }
  }
  FUNC_EPILOGUE;
  return i0;
}

static u32 f9(u32 p0) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = 0u;
  (*Z_envZ_abortZ_vi)(i0);
  i0 = 0u;
  FUNC_EPILOGUE;
  return i0;
}

static u32 f10(u32 p0) {
  u32 l0 = 0, l1 = 0, l2 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2;
  i0 = p0;
  l1 = i0;
  i1 = 3u;
  i0 &= i1;
  if (i0) {
    i0 = l1;
    l0 = i0;
    p0 = i0;
    L2: 
      i0 = l0;
      i0 = i32_load8_s(Z_envZ_memory, (u64)(i0));
      i0 = !(i0);
      if (i0) {goto B0;}
      i0 = l0;
      i1 = 1u;
      i0 += i1;
      l0 = i0;
      p0 = i0;
      i1 = 3u;
      i0 &= i1;
      if (i0) {goto L2;}
    i0 = l0;
    p0 = i0;
  }
  L3: 
    i0 = p0;
    i1 = 4u;
    i0 += i1;
    l0 = i0;
    i0 = p0;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l2 = i0;
    i1 = 2155905152u;
    i0 &= i1;
    i1 = 2155905152u;
    i0 ^= i1;
    i1 = l2;
    i2 = 4278124287u;
    i1 += i2;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {
      i0 = l0;
      p0 = i0;
      goto L3;
    }
  i0 = l2;
  i1 = 255u;
  i0 &= i1;
  if (i0) {
    L6: 
      i0 = p0;
      i1 = 1u;
      i0 += i1;
      p0 = i0;
      i0 = i32_load8_s(Z_envZ_memory, (u64)(i0));
      if (i0) {goto L6;}
  }
  B0:;
  i0 = p0;
  i1 = l1;
  i0 -= i1;
  FUNC_EPILOGUE;
  return i0;
}

static u32 ___errno_location(void) {
  FUNC_PROLOGUE;
  u32 i0;
  i0 = 1524u;
  FUNC_EPILOGUE;
  return i0;
}

static void _free(u32 p0) {
  u32 l0 = 0, l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1, i2, i3, i4;
  i0 = p0;
  i0 = !(i0);
  if (i0) {
    goto Bfunc;
  }
  i0 = 1044u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l3 = i0;
  i0 = p0;
  i1 = 4294967288u;
  i0 += i1;
  l1 = i0;
  i1 = p0;
  i2 = 4294967292u;
  i1 += i2;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  l2 = i1;
  i2 = 4294967288u;
  i1 &= i2;
  p0 = i1;
  i0 += i1;
  l4 = i0;
  i0 = l2;
  i1 = 1u;
  i0 &= i1;
  if (i0) {
    i0 = l1;
  } else {
    i0 = l1;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l0 = i0;
    i0 = l2;
    i1 = 3u;
    i0 &= i1;
    i0 = !(i0);
    if (i0) {
      goto Bfunc;
    }
    i0 = l1;
    i1 = l0;
    i0 -= i1;
    l1 = i0;
    i1 = l3;
    i0 = i0 < i1;
    if (i0) {
      goto Bfunc;
    }
    i0 = l0;
    i1 = p0;
    i0 += i1;
    p0 = i0;
    i0 = 1048u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    i1 = l1;
    i0 = i0 == i1;
    if (i0) {
      i0 = l1;
      i1 = l4;
      i2 = 4u;
      i1 += i2;
      l0 = i1;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      l2 = i1;
      i2 = 3u;
      i1 &= i2;
      i2 = 3u;
      i1 = i1 != i2;
      if (i1) {goto B1;}
      i0 = 1036u;
      i1 = p0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l0;
      i1 = l2;
      i2 = 4294967294u;
      i1 &= i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l1;
      i1 = p0;
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l1;
      i1 = p0;
      i0 += i1;
      i1 = p0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      goto Bfunc;
    }
    i0 = l0;
    i1 = 3u;
    i0 >>= (i1 & 31);
    l3 = i0;
    i0 = l0;
    i1 = 256u;
    i0 = i0 < i1;
    if (i0) {
      i0 = l1;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
      l0 = i0;
      i1 = l1;
      i1 = i32_load(Z_envZ_memory, (u64)(i1 + 8));
      l2 = i1;
      i0 = i0 == i1;
      if (i0) {
        i0 = 1028u;
        i1 = 1028u;
        i1 = i32_load(Z_envZ_memory, (u64)(i1));
        i2 = 1u;
        i3 = l3;
        i2 <<= (i3 & 31);
        i3 = 4294967295u;
        i2 ^= i3;
        i1 &= i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
      } else {
        i0 = l2;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = l0;
        i1 = l2;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
      }
      i0 = l1;
      goto B1;
    }
    i0 = l1;
    i0 = i32_load(Z_envZ_memory, (u64)(i0 + 24));
    l6 = i0;
    i0 = l1;
    i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
    l0 = i0;
    i1 = l1;
    i0 = i0 == i1;
    if (i0) {
      i0 = l1;
      i1 = 16u;
      i0 += i1;
      l2 = i0;
      i1 = 4u;
      i0 += i1;
      l3 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l0 = i0;
      if (i0) {
        i0 = l3;
        l2 = i0;
      } else {
        i0 = l2;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l0 = i0;
        i0 = !(i0);
        if (i0) {
          i0 = 0u;
          l0 = i0;
          goto B8;
        }
      }
      L12: 
        i0 = l0;
        i1 = 20u;
        i0 += i1;
        l3 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l5 = i0;
        i0 = !(i0);
        if (i0) {
          i0 = l0;
          i1 = 16u;
          i0 += i1;
          l3 = i0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          l5 = i0;
          i0 = !(i0);
          if (i0) {goto B13;}
        }
        i0 = l3;
        l2 = i0;
        i0 = l5;
        l0 = i0;
        goto L12;
        B13:;
      i0 = l2;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
    } else {
      i0 = l1;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
      l2 = i0;
      i1 = l0;
      i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
      i0 = l0;
      i1 = l2;
      i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
    }
    B8:;
    i0 = l6;
    if (i0) {
      i0 = l1;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 28));
      l2 = i0;
      i1 = 2u;
      i0 <<= (i1 & 31);
      i1 = 1332u;
      i0 += i1;
      l3 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      i1 = l1;
      i0 = i0 == i1;
      if (i0) {
        i0 = l3;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l0;
        i0 = !(i0);
        if (i0) {
          i0 = 1032u;
          i1 = 1032u;
          i1 = i32_load(Z_envZ_memory, (u64)(i1));
          i2 = 1u;
          i3 = l2;
          i2 <<= (i3 & 31);
          i3 = 4294967295u;
          i2 ^= i3;
          i1 &= i2;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = l1;
          goto B1;
        }
      } else {
        i0 = l6;
        i1 = 20u;
        i0 += i1;
        l2 = i0;
        i0 = l6;
        i1 = 16u;
        i0 += i1;
        l3 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        i1 = l1;
        i0 = i0 == i1;
        if (i0) {
          i0 = l3;
        } else {
          i0 = l2;
        }
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
        i0 = l1;
        i1 = l0;
        i1 = !(i1);
        if (i1) {goto B1;}
      }
      i0 = l0;
      i1 = l6;
      i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
      i0 = l1;
      i1 = 16u;
      i0 += i1;
      l3 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
      l2 = i0;
      if (i0) {
        i0 = l0;
        i1 = l2;
        i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
        i0 = l2;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
      }
      i0 = l3;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
      l2 = i0;
      if (i0) {
        i0 = l0;
        i1 = l2;
        i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
        i0 = l2;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
      }
      i0 = l1;
    } else {
      i0 = l1;
    }
  }
  B1:;
  l6 = i0;
  i1 = l4;
  i0 = i0 >= i1;
  if (i0) {
    goto Bfunc;
  }
  i0 = l4;
  i1 = 4u;
  i0 += i1;
  l2 = i0;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l0 = i0;
  i1 = 1u;
  i0 &= i1;
  i0 = !(i0);
  if (i0) {
    goto Bfunc;
  }
  i0 = l0;
  i1 = 2u;
  i0 &= i1;
  if (i0) {
    i0 = l2;
    i1 = l0;
    i2 = 4294967294u;
    i1 &= i2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l1;
    i1 = p0;
    i2 = 1u;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = l6;
    i1 = p0;
    i0 += i1;
    i1 = p0;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = p0;
    l2 = i0;
  } else {
    i0 = 1052u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    i1 = l4;
    i0 = i0 == i1;
    if (i0) {
      i0 = 1040u;
      i1 = 1040u;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = p0;
      i1 += i2;
      p0 = i1;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1052u;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l1;
      i1 = p0;
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l1;
      i1 = 1048u;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i0 = i0 != i1;
      if (i0) {
        goto Bfunc;
      }
      i0 = 1048u;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1036u;
      i1 = 0u;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      goto Bfunc;
    }
    i0 = 1048u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    i1 = l4;
    i0 = i0 == i1;
    if (i0) {
      i0 = 1036u;
      i1 = 1036u;
      i1 = i32_load(Z_envZ_memory, (u64)(i1));
      i2 = p0;
      i1 += i2;
      p0 = i1;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = 1048u;
      i1 = l6;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l1;
      i1 = p0;
      i2 = 1u;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
      i0 = l6;
      i1 = p0;
      i0 += i1;
      i1 = p0;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      goto Bfunc;
    }
    i0 = l0;
    i1 = 4294967288u;
    i0 &= i1;
    i1 = p0;
    i0 += i1;
    l2 = i0;
    i0 = l0;
    i1 = 3u;
    i0 >>= (i1 & 31);
    l3 = i0;
    i0 = l0;
    i1 = 256u;
    i0 = i0 < i1;
    if (i0) {
      i0 = l4;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
      p0 = i0;
      i1 = l4;
      i1 = i32_load(Z_envZ_memory, (u64)(i1 + 8));
      l0 = i1;
      i0 = i0 == i1;
      if (i0) {
        i0 = 1028u;
        i1 = 1028u;
        i1 = i32_load(Z_envZ_memory, (u64)(i1));
        i2 = 1u;
        i3 = l3;
        i2 <<= (i3 & 31);
        i3 = 4294967295u;
        i2 ^= i3;
        i1 &= i2;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
      } else {
        i0 = l0;
        i1 = p0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = p0;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
      }
    } else {
      i0 = l4;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 24));
      l7 = i0;
      i0 = l4;
      i0 = i32_load(Z_envZ_memory, (u64)(i0 + 12));
      p0 = i0;
      i1 = l4;
      i0 = i0 == i1;
      if (i0) {
        i0 = l4;
        i1 = 16u;
        i0 += i1;
        l0 = i0;
        i1 = 4u;
        i0 += i1;
        l3 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        p0 = i0;
        if (i0) {
          i0 = l3;
          l0 = i0;
        } else {
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          p0 = i0;
          i0 = !(i0);
          if (i0) {
            i0 = 0u;
            p0 = i0;
            goto B30;
          }
        }
        L34: 
          i0 = p0;
          i1 = 20u;
          i0 += i1;
          l3 = i0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          l5 = i0;
          i0 = !(i0);
          if (i0) {
            i0 = p0;
            i1 = 16u;
            i0 += i1;
            l3 = i0;
            i0 = i32_load(Z_envZ_memory, (u64)(i0));
            l5 = i0;
            i0 = !(i0);
            if (i0) {goto B35;}
          }
          i0 = l3;
          l0 = i0;
          i0 = l5;
          p0 = i0;
          goto L34;
          B35:;
        i0 = l0;
        i1 = 0u;
        i32_store(Z_envZ_memory, (u64)(i0), i1);
      } else {
        i0 = l4;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 8));
        l0 = i0;
        i1 = p0;
        i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
        i0 = p0;
        i1 = l0;
        i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
      }
      B30:;
      i0 = l7;
      if (i0) {
        i0 = l4;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 28));
        l0 = i0;
        i1 = 2u;
        i0 <<= (i1 & 31);
        i1 = 1332u;
        i0 += i1;
        l3 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        i1 = l4;
        i0 = i0 == i1;
        if (i0) {
          i0 = l3;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = p0;
          i0 = !(i0);
          if (i0) {
            i0 = 1032u;
            i1 = 1032u;
            i1 = i32_load(Z_envZ_memory, (u64)(i1));
            i2 = 1u;
            i3 = l0;
            i2 <<= (i3 & 31);
            i3 = 4294967295u;
            i2 ^= i3;
            i1 &= i2;
            i32_store(Z_envZ_memory, (u64)(i0), i1);
            goto B27;
          }
        } else {
          i0 = l7;
          i1 = 20u;
          i0 += i1;
          l0 = i0;
          i0 = l7;
          i1 = 16u;
          i0 += i1;
          l3 = i0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0));
          i1 = l4;
          i0 = i0 == i1;
          if (i0) {
            i0 = l3;
          } else {
            i0 = l0;
          }
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0), i1);
          i0 = p0;
          i0 = !(i0);
          if (i0) {goto B27;}
        }
        i0 = p0;
        i1 = l7;
        i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
        i0 = l4;
        i1 = 16u;
        i0 += i1;
        l3 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l0 = i0;
        if (i0) {
          i0 = p0;
          i1 = l0;
          i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
          i0 = l0;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
        }
        i0 = l3;
        i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
        l0 = i0;
        if (i0) {
          i0 = p0;
          i1 = l0;
          i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
          i0 = l0;
          i1 = p0;
          i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
        }
      }
    }
    B27:;
    i0 = l1;
    i1 = l2;
    i2 = 1u;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0 + 4), i1);
    i0 = l6;
    i1 = l2;
    i0 += i1;
    i1 = l2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l1;
    i1 = 1048u;
    i1 = i32_load(Z_envZ_memory, (u64)(i1));
    i0 = i0 == i1;
    if (i0) {
      i0 = 1036u;
      i1 = l2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      goto Bfunc;
    }
  }
  i0 = l2;
  i1 = 3u;
  i0 >>= (i1 & 31);
  l0 = i0;
  i0 = l2;
  i1 = 256u;
  i0 = i0 < i1;
  if (i0) {
    i0 = l0;
    i1 = 3u;
    i0 <<= (i1 & 31);
    i1 = 1068u;
    i0 += i1;
    p0 = i0;
    i0 = 1028u;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l2 = i0;
    i1 = 1u;
    i2 = l0;
    i1 <<= (i2 & 31);
    l0 = i1;
    i0 &= i1;
    if (i0) {
      i0 = p0;
      i1 = 8u;
      i0 += i1;
      l2 = i0;
      i0 = i32_load(Z_envZ_memory, (u64)(i0));
    } else {
      i0 = 1028u;
      i1 = l2;
      i2 = l0;
      i1 |= i2;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = p0;
      i1 = 8u;
      i0 += i1;
      l2 = i0;
      i0 = p0;
    }
    l0 = i0;
    i0 = l2;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l0;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
    i0 = l1;
    i1 = l0;
    i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
    i0 = l1;
    i1 = p0;
    i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
    goto Bfunc;
  }
  i0 = l2;
  i1 = 8u;
  i0 >>= (i1 & 31);
  p0 = i0;
  if (i0) {
    i0 = l2;
    i1 = 16777215u;
    i0 = i0 > i1;
    if (i0) {
      i0 = 31u;
    } else {
      i0 = l2;
      i1 = 14u;
      i2 = p0;
      i3 = p0;
      i4 = 1048320u;
      i3 += i4;
      i4 = 16u;
      i3 >>= (i4 & 31);
      i4 = 8u;
      i3 &= i4;
      p0 = i3;
      i2 <<= (i3 & 31);
      l0 = i2;
      i3 = 520192u;
      i2 += i3;
      i3 = 16u;
      i2 >>= (i3 & 31);
      i3 = 4u;
      i2 &= i3;
      l3 = i2;
      i3 = p0;
      i2 |= i3;
      i3 = l0;
      i4 = l3;
      i3 <<= (i4 & 31);
      p0 = i3;
      i4 = 245760u;
      i3 += i4;
      i4 = 16u;
      i3 >>= (i4 & 31);
      i4 = 2u;
      i3 &= i4;
      l0 = i3;
      i2 |= i3;
      i1 -= i2;
      i2 = p0;
      i3 = l0;
      i2 <<= (i3 & 31);
      i3 = 15u;
      i2 >>= (i3 & 31);
      i1 += i2;
      p0 = i1;
      i2 = 7u;
      i1 += i2;
      i0 >>= (i1 & 31);
      i1 = 1u;
      i0 &= i1;
      i1 = p0;
      i2 = 1u;
      i1 <<= (i2 & 31);
      i0 |= i1;
    }
  } else {
    i0 = 0u;
  }
  l0 = i0;
  i1 = 2u;
  i0 <<= (i1 & 31);
  i1 = 1332u;
  i0 += i1;
  p0 = i0;
  i0 = l1;
  i1 = l0;
  i32_store(Z_envZ_memory, (u64)(i0 + 28), i1);
  i0 = l1;
  i1 = 0u;
  i32_store(Z_envZ_memory, (u64)(i0 + 20), i1);
  i0 = l1;
  i1 = 0u;
  i32_store(Z_envZ_memory, (u64)(i0 + 16), i1);
  i0 = 1032u;
  i0 = i32_load(Z_envZ_memory, (u64)(i0));
  l3 = i0;
  i1 = 1u;
  i2 = l0;
  i1 <<= (i2 & 31);
  l5 = i1;
  i0 &= i1;
  if (i0) {
    i0 = p0;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    p0 = i0;
    i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
    i1 = 4294967288u;
    i0 &= i1;
    i1 = l2;
    i0 = i0 == i1;
    if (i0) {
      i0 = p0;
    } else {
      i0 = 25u;
      i1 = l0;
      i2 = 1u;
      i1 >>= (i2 & 31);
      i0 -= i1;
      l3 = i0;
      i0 = l2;
      i1 = l0;
      i2 = 31u;
      i1 = i1 == i2;
      if (i1) {
        i1 = 0u;
      } else {
        i1 = l3;
      }
      i0 <<= (i1 & 31);
      l3 = i0;
      L53: 
        i0 = p0;
        i1 = 16u;
        i0 += i1;
        i1 = l3;
        i2 = 31u;
        i1 >>= (i2 & 31);
        i2 = 2u;
        i1 <<= (i2 & 31);
        i0 += i1;
        l5 = i0;
        i0 = i32_load(Z_envZ_memory, (u64)(i0));
        l0 = i0;
        if (i0) {
          i0 = l3;
          i1 = 1u;
          i0 <<= (i1 & 31);
          l3 = i0;
          i0 = l0;
          i0 = i32_load(Z_envZ_memory, (u64)(i0 + 4));
          i1 = 4294967288u;
          i0 &= i1;
          i1 = l2;
          i0 = i0 == i1;
          if (i0) {goto B50;}
          i0 = l0;
          p0 = i0;
          goto L53;
        }
      i0 = l5;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0), i1);
      i0 = l1;
      i1 = p0;
      i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
      i0 = l1;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
      i0 = l1;
      i1 = l1;
      i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
      goto B48;
    }
    l0 = i0;
    B50:;
    i0 = l0;
    i1 = 8u;
    i0 += i1;
    p0 = i0;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l2 = i0;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
    i0 = p0;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l1;
    i1 = l2;
    i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
    i0 = l1;
    i1 = l0;
    i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
    i0 = l1;
    i1 = 0u;
    i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
  } else {
    i0 = 1032u;
    i1 = l3;
    i2 = l5;
    i1 |= i2;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = p0;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0), i1);
    i0 = l1;
    i1 = p0;
    i32_store(Z_envZ_memory, (u64)(i0 + 24), i1);
    i0 = l1;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0 + 12), i1);
    i0 = l1;
    i1 = l1;
    i32_store(Z_envZ_memory, (u64)(i0 + 8), i1);
  }
  B48:;
  i0 = 1060u;
  i1 = 1060u;
  i1 = i32_load(Z_envZ_memory, (u64)(i1));
  i2 = 4294967295u;
  i1 += i2;
  p0 = i1;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  i0 = p0;
  if (i0) {
    goto Bfunc;
  }
  i0 = 1484u;
  p0 = i0;
  L56: 
    i0 = p0;
    i0 = i32_load(Z_envZ_memory, (u64)(i0));
    l1 = i0;
    i1 = 8u;
    i0 += i1;
    p0 = i0;
    i0 = l1;
    if (i0) {goto L56;}
  i0 = 1060u;
  i1 = 4294967295u;
  i32_store(Z_envZ_memory, (u64)(i0), i1);
  Bfunc:;
  FUNC_EPILOGUE;
}

static const u8 data_segment_data_0[] = {
  0x01, 
};

static void init_memory(void) {
  memcpy(&((*Z_envZ_memory).data[1024u]), data_segment_data_0, 1);
}

static void init_table(void) {
  uint32_t offset;
  offset = (*Z_envZ_tableBaseZ_i);
  (*Z_envZ_table).data[offset + 0] = (wasm_rt_elem_t){func_types[0], (wasm_rt_anyfunc_t)(&f9)};
  (*Z_envZ_table).data[offset + 1] = (wasm_rt_elem_t){func_types[0], (wasm_rt_anyfunc_t)(&f8)};
}

/* export: '___errno_location' */
u32 (*WASM_RT_ADD_PREFIX(Z____errno_locationZ_iv))(void);
/* export: '_colorFromHexString' */
void (*WASM_RT_ADD_PREFIX(Z__colorFromHexStringZ_vii))(u32, u32);
/* export: '_free' */
void (*WASM_RT_ADD_PREFIX(Z__freeZ_vi))(u32);
/* export: '_malloc' */
u32 (*WASM_RT_ADD_PREFIX(Z__mallocZ_ii))(u32);

static void init_exports(void) {
  /* export: '___errno_location' */
  WASM_RT_ADD_PREFIX(Z____errno_locationZ_iv) = (&___errno_location);
  /* export: '_colorFromHexString' */
  WASM_RT_ADD_PREFIX(Z__colorFromHexStringZ_vii) = (&_colorFromHexString);
  /* export: '_free' */
  WASM_RT_ADD_PREFIX(Z__freeZ_vi) = (&_free);
  /* export: '_malloc' */
  WASM_RT_ADD_PREFIX(Z__mallocZ_ii) = (&_malloc);
}

void WASM_RT_ADD_PREFIX(init)(void) {
  init_func_types();
  init_globals();
  init_memory();
  init_table();
  init_exports();
}

void WASM_RT_ADD_PREFIX(init_globals_)(void) {
  init_globals();
}

