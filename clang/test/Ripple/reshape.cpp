// REQUIRES: hexagon-registered-target || aarch64-registered-target || x86-registered-target
// RUN: %clang++ -S -fenable-ripple -O2 -Rpass-missed=ripple -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not="warning:" --implicit-check-not="remark:" --implicit-check-not="error:"

#include "ripple_test.h"

#define VECTOR_LANE 0

// Reshape a 1D block of 32 int32 elements into a 2D 8x4 view and store it.
// The flat element count is identical (32), so the reshape is optimized away.
//
// CHECK-NOT:   ripple.reshape
// CHECK:       load <32 x i32>
// CHECK:       store <32 x i32>
// CHECK-NOT:   bitcast
void reshape_1d_to_2d(int32_t input[32], int32_t output[32]) {
  // Source shape: 1D, 32 elements
  ripple_block_t BS_src = ripple_set_block_shape(VECTOR_LANE, 32);
  size_t v0_src = ripple_id(BS_src, 0);

  // Load a flat 1D block
  int32_t val = input[v0_src];

  // Target shape: 2D, 8 x 4 = 32 elements
  ripple_block_t BS_dst = ripple_set_block_shape(VECTOR_LANE, 8, 4);
  size_t v0_dst = ripple_id(BS_dst, 0);
  size_t v1_dst = ripple_id(BS_dst, 1);

  // Reshape: reinterpret the same 32 elements under the new 2D layout
  int32_t reshaped = ripple_reshape(BS_dst, val);

  output[v0_dst + 8 * v1_dst] = reshaped;
}

// Reshape a 2D 4x8 float block into a 1D 32-element view.
// Verifies the symmetric (2D -> 1D) direction is also optimized away.
//
// CHECK-NOT:   ripple.reshape
// CHECK:       load <32 x float>
// CHECK:       store <32 x float>
// CHECK-NOT:   bitcast
void reshape_2d_to_1d(float input[32], float output[32]) {
  // Source shape: 2D, 4 x 8 = 32 elements
  ripple_block_t BS_src = ripple_set_block_shape(VECTOR_LANE, 4, 8);
  size_t v0_src = ripple_id(BS_src, 0);
  size_t v1_src = ripple_id(BS_src, 1);

  float val = input[v0_src + 4 * v1_src];

  // Target shape: 1D, 32 elements
  ripple_block_t BS_dst = ripple_set_block_shape(VECTOR_LANE, 32);
  size_t v0_dst = ripple_id(BS_dst, 0);

  float reshaped = ripple_reshape(BS_dst, val);

  output[v0_dst] = reshaped;
}

// Reshape a 1D block of 16 int16 elements into a 2D 4x4 view.
//
// CHECK-NOT:   ripple.reshape
// CHECK:       load <16 x i16>
// CHECK:       store <16 x i16>
// CHECK-NOT:   bitcast
void reshape_i16_1d_to_2d(int16_t input[16], int16_t output[16]) {
  ripple_block_t BS_src = ripple_set_block_shape(VECTOR_LANE, 16);
  size_t v0_src = ripple_id(BS_src, 0);

  int16_t val = input[v0_src];

  ripple_block_t BS_dst = ripple_set_block_shape(VECTOR_LANE, 4, 4);
  size_t v0_dst = ripple_id(BS_dst, 0);
  size_t v1_dst = ripple_id(BS_dst, 1);

  int16_t reshaped = ripple_reshape(BS_dst, val);

  output[v0_dst + 4 * v1_dst] = reshaped;
}

// Reshaping a loop-carried value must preserve the original PHI's shape.
// The flat LLVM vector type is identical for both views; reusing the PHI as
// the reshape result used to overwrite its shape and fail during PHI fixup.
//
// CHECK-LABEL: define{{.*}} void @reshape_loop(
// CHECK-DAG:   phi <32 x float>
// CHECK-DAG:   shufflevector <32 x float>
// CHECK-DAG:   fadd <32 x float>
// CHECK-DAG:   store <32 x float>
// CHECK-NOT:   ripple.reshape
static size_t rotate_two(size_t dst, size_t n) { return (dst + 2) % n; }
extern "C" void reshape_loop(const float *input, float *output, unsigned steps) {
  ripple_block_t B = ripple_set_block_shape(VECTOR_LANE, 2, 16);
  ripple_block_t flatB = ripple_set_block_shape(VECTOR_LANE, 32);
  size_t index = 2 * ripple_id(B, 1) + ripple_id(B, 0);
  float value = input[index];
  for (unsigned i = 0; i < steps; ++i) {
    float flat = ripple_reshape(flatB, value);
    float rotated = ripple_shuffle(flat, rotate_two);
    value += ripple_reshape(B, rotated);
  }
  output[index] = value;
}
