#include <arm_sme.h>

#define RIPPLE_API_FIXED_VSCALE 4

// Verify if Ripple operation fixed vscale is same as runtime vscale.
bool verify_ripple_sme_api_fixed_vscale() {
  if ((svcntw() / sizeof(float)) == RIPPLE_API_FIXED_VSCALE)
    return true;
  else
    return false;
}

// There is no macro for streaming SVE bits, unlike __ARM_FEATURE_SVE_BITS.
#define SVE_BYTES (512 / 8)
#define VEC_ELEMS_I8 (SVE_BYTES / sizeof(int8_t))
#define VEC_ELEMS_I32 (SVE_BYTES / sizeof(int32_t))

#define RIPPLE_LIB_ATTR                                                        \
  inline __attribute__((used, always_inline, weak, visibility("hidden")))
#define UTIL_FUNC_ATTR static inline __attribute__((always_inline))

// Vector types.
typedef int8_t NEON_SSVL_I8_VT __attribute__((__vector_size__(SVE_BYTES)));
typedef int8_t NEON_SSVL_I8_VT_x2
    __attribute__((__vector_size__(SVE_BYTES * 2)));
typedef int32_t NEON_SSVL_I32_VT __attribute__((__vector_size__(SVE_BYTES)));
typedef int32_t NEON_SSVL_I32_VT_x2
    __attribute__((__vector_size__(SVE_BYTES * 2)));
typedef float NEON_SSVL_F32_VT __attribute__((__vector_size__(SVE_BYTES)));
typedef float NEON_SSVL_F32_VT_x2
    __attribute__((__vector_size__(SVE_BYTES * 2)));

// Matrix/2d vector types.
typedef int32_t NEON_SSVL_MT_I32
    __attribute__((__vector_size__(SVE_BYTES * VEC_ELEMS_I32)));
typedef int32_t NEON_SSVL_MT_I32_x4
    __attribute__((__vector_size__(SVE_BYTES * 2 * VEC_ELEMS_I32 * 2)));
typedef float NEON_SSVL_MT_F32
    __attribute__((__vector_size__(SVE_BYTES * VEC_ELEMS_I32)));
typedef float NEON_SSVL_MT_F32_x4
    __attribute__((__vector_size__(SVE_BYTES * 2 * VEC_ELEMS_I32 * 2)));

union DoubleVector {
  NEON_SSVL_I8_VT_x2 I8Vector;
  NEON_SSVL_I32_VT_x2 I32Vector;
  NEON_SSVL_F32_VT_x2 F32Vector;
  NEON_SSVL_I8_VT I8Vectors[2];
  NEON_SSVL_I32_VT I32Vectors[2];
  NEON_SSVL_F32_VT F32Vectors[2];
};
typedef union DoubleVector DoubleVector;
///////////////  Utility functions  //////////////
// Converting to/from scalable/fixed.
UTIL_FUNC_ATTR NEON_SSVL_I32_VT to_fixed_i32(svint32_t x) __arm_streaming {
  NEON_SSVL_I32_VT out;
  svst1_s32(svptrue_b32(), (int32_t *)(&out), x);
  return out;
}

UTIL_FUNC_ATTR svint32_t
from_fixed_i32(const NEON_SSVL_I32_VT *p) __arm_streaming {
  return svld1_s32(svptrue_b32(), (const int32_t *)(p));
}

UTIL_FUNC_ATTR NEON_SSVL_I8_VT to_fixed_i8(svint8_t x) __arm_streaming {
  NEON_SSVL_I8_VT out;
  svst1_s8(svptrue_b8(), (int8_t *)(&out), x);
  return out;
}

UTIL_FUNC_ATTR svint8_t
from_fixed_i8(const NEON_SSVL_I8_VT *p) __arm_streaming {
  return svld1_s8(svptrue_b8(), (const int8_t *)(p));
}

UTIL_FUNC_ATTR NEON_SSVL_F32_VT to_fixed_f32(svfloat32_t x) __arm_streaming {
  NEON_SSVL_F32_VT out;
  svst1_f32(svptrue_b32(), (float *)(&out), x);
  return out;
}

UTIL_FUNC_ATTR svfloat32_t
from_fixed_f32(const NEON_SSVL_F32_VT *p) __arm_streaming {
  return svld1_f32(svptrue_b32(), (const float *)(p));
}

///////////////  SME library functions  //////////////

/// Zero the tiles. Dummy return value.
RIPPLE_LIB_ATTR NEON_SSVL_MT_I32_x4
ripple_ret_t32x32i32_zeroAccumulator_i32(void) __arm_streaming __arm_out(
    "za") {
  svzero_za();
  return (NEON_SSVL_MT_I32_x4){0};
}

/// Zero the tiles. Dummy return value.
RIPPLE_LIB_ATTR NEON_SSVL_MT_F32_x4
ripple_ret_t32x32f32_zeroAccumulator_f32(void) __arm_streaming __arm_out(
    "za") {
  svzero_za();
  return (NEON_SSVL_MT_F32_x4){0.0f};
}

/// Add two I32 vectors to horizontally to all rows in the SME tiles.
RIPPLE_LIB_ATTR void ripple_arg0_t32i32_addHorizToAccumulator_i32(
    NEON_SSVL_I32_VT_x2 X) __arm_streaming __arm_inout("za") {
  svbool_t PTrue = svptrue_b8();
  NEON_SSVL_I32_VT X_lo = (DoubleVector){X}.I32Vectors[0];
  NEON_SSVL_I32_VT X_hi = (DoubleVector){X}.I32Vectors[1];
  svaddha_za32_s32_m(0, PTrue, PTrue, from_fixed_i32(&X_lo));
  svaddha_za32_s32_m(1, PTrue, PTrue, from_fixed_i32(&X_hi));
  svaddha_za32_s32_m(2, PTrue, PTrue, from_fixed_i32(&X_lo));
  svaddha_za32_s32_m(3, PTrue, PTrue, from_fixed_i32(&X_hi));
}

/// Outer Product Accumulate. 4-way I8->I32 signed.
RIPPLE_LIB_ATTR void
ripple_ew_arg0_t128i8_arg1_t128i8_outerProductAccumulate_i8(
    NEON_SSVL_I8_VT_x2 X,
    NEON_SSVL_I8_VT_x2 Y) __arm_streaming __arm_inout("za") {
  svbool_t PTrue = svptrue_b8();
  NEON_SSVL_I8_VT LHS_lo = (DoubleVector){X}.I8Vectors[0];
  NEON_SSVL_I8_VT LHS_hi = (DoubleVector){X}.I8Vectors[1];

  NEON_SSVL_I8_VT RHS_lo = (DoubleVector){Y}.I8Vectors[0];
  NEON_SSVL_I8_VT RHS_hi = (DoubleVector){Y}.I8Vectors[1];

  svmopa_za32_m(0, PTrue, PTrue, from_fixed_i8(&LHS_lo),
                from_fixed_i8(&RHS_lo));
  svmopa_za32_m(1, PTrue, PTrue, from_fixed_i8(&LHS_lo),
                from_fixed_i8(&RHS_hi));
  svmopa_za32_m(2, PTrue, PTrue, from_fixed_i8(&LHS_hi),
                from_fixed_i8(&RHS_lo));
  svmopa_za32_m(3, PTrue, PTrue, from_fixed_i8(&LHS_hi),
                from_fixed_i8(&RHS_hi));
}

/// Outer Product Accumulate. F32.
RIPPLE_LIB_ATTR void
ripple_ew_arg0_t32f32_arg1_t32f32_outerProductAccumulate_f32(
    NEON_SSVL_F32_VT_x2 X,
    NEON_SSVL_F32_VT_x2 Y) __arm_streaming __arm_inout("za") {
  svbool_t PTrue = svptrue_b32();
  NEON_SSVL_F32_VT LHS_lo = (DoubleVector){X}.F32Vectors[0];
  NEON_SSVL_F32_VT LHS_hi = (DoubleVector){X}.F32Vectors[1];

  NEON_SSVL_F32_VT RHS_lo = (DoubleVector){Y}.F32Vectors[0];
  NEON_SSVL_F32_VT RHS_hi = (DoubleVector){Y}.F32Vectors[1];

  svmopa_za32_m(0, PTrue, PTrue, from_fixed_f32(&LHS_lo),
                from_fixed_f32(&RHS_lo));
  svmopa_za32_m(1, PTrue, PTrue, from_fixed_f32(&LHS_lo),
                from_fixed_f32(&RHS_hi));
  svmopa_za32_m(2, PTrue, PTrue, from_fixed_f32(&LHS_hi),
                from_fixed_f32(&RHS_lo));
  svmopa_za32_m(3, PTrue, PTrue, from_fixed_f32(&LHS_hi),
                from_fixed_f32(&RHS_hi));
}

/// Get horizontal slice (2 vectors) from the SME tile.
/// Tile has to be accessed using a constant integer name (za0, za1...).
/// There are 4 tiles for 32-bit data type, and just one for 8bit.
/// We could also have a branch on the "slice" number, or select based on a
/// condition, but getting a variable index from the single za0 tile and
/// reinterpreting to 32 bit results in better code. The tiles are visually
/// abstracted to something like this:
/// +-----+-----+
/// | za0 | za1 |
/// +-----+-----+
/// | za2 | za3 |
/// +-----+-----+
/// But in reality, for 32bit data (.s) the layout is:
/// za0.s[0]  ==  za0.b[0]
/// za1.s[0]  ==  za0.b[1]
/// za2.s[0]  ==  za0.b[2]
/// za3.s[0]  ==  za0.b[3]
/// za0.s[1]  ==  za0.b[4]
/// za1.s[1]  ==  za0.b[5]
/// za2.s[1]  ==  za0.b[6]
/// za3.s[1]  ==  za0.b[7]
/// We want the function to return {za0.s[x], za1.s[x]}
/// or if x >= VEC_ELEMS_I32 {za2.s[x], za3.s[x]}
/// Slice <= VEC_ELEMS_I32 or it's an out of bounds tile access.
RIPPLE_LIB_ATTR NEON_SSVL_I32_VT_x2
ripple_pure_ret_t32i32_getAccumHorizSlice_i32(
    uint32_t Slice) __arm_streaming __arm_out("za") {
  uint32_t ByteTileIdx = (Slice % VEC_ELEMS_I32) * 4;
  // For za2 and za3 slice is >= VEC_ELEMS_I32, so effectively add 2.
  ByteTileIdx += (Slice / VEC_ELEMS_I32) * 2;

  svbool_t PTrue = svptrue_b8();
  DoubleVector DoubleVec;
  DoubleVec.I32Vectors[0] = to_fixed_i32(svreinterpret_s32_s8(
      svread_hor_za8_m(svdup_s8(0), PTrue, 0, ByteTileIdx)));
  DoubleVec.I32Vectors[1] = to_fixed_i32(svreinterpret_s32_s8(
      svread_hor_za8_m(svdup_s8(0), PTrue, 0, ByteTileIdx + 1)));
  return (NEON_SSVL_I32_VT_x2)DoubleVec.I32Vector;
}

/// Get horizontal slice (2 vectors) from the SME tile.
RIPPLE_LIB_ATTR NEON_SSVL_F32_VT_x2
ripple_pure_ret_t32f32_getAccumHorizSlice_f32(
    uint32_t Slice) __arm_streaming __arm_out("za") {
  uint32_t ByteTileIdx = (Slice % VEC_ELEMS_I32) * 4;
  // For za2 and za3 slice is >= VEC_ELEMS_I32, so effectively add 2.
  ByteTileIdx += (Slice / VEC_ELEMS_I32) * 2;

  svbool_t PTrue = svptrue_b32();
  DoubleVector DoubleVec;
  DoubleVec.F32Vectors[0] = to_fixed_f32(svreinterpret_f32_s8(
      svread_hor_za8_m(svdup_s8(0), PTrue, 0, ByteTileIdx)));
  DoubleVec.F32Vectors[1] = to_fixed_f32(svreinterpret_f32_s8(
      svread_hor_za8_m(svdup_s8(0), PTrue, 0, ByteTileIdx + 1)));
  return (NEON_SSVL_F32_VT_x2)DoubleVec.F32Vector;
}