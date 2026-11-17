// REQUIRES: hexagon-registered-target
// Disable loop unrolling to check a single guarded SDK call per fallback loop.
// RUN: %clang -O2 -fno-unroll-loops -S -emit-llvm -fenable-ripple \
// RUN:   --target=hexagon -mv81 -mhvx -mhvx-ieee-fp -ffreestanding %s -o - \
// RUN:   | FileCheck %s
// RUN: %clang -x c++ -O2 -fno-unroll-loops -S -emit-llvm -fenable-ripple \
// RUN:   --target=hexagon -mv81 -mhvx -mhvx-ieee-fp -ffreestanding %s -o - \
// RUN:   | FileCheck %s
// RUN: %clang -O2 -fno-unroll-loops -S -emit-llvm -fenable-ripple \
// RUN:   --target=hexagon -mv68 -mhvx -mhvx-qfloat -ffreestanding \
// RUN:   -Wno-nan-infinity-disabled %s -o - | FileCheck %s
// RUN: %clang -x c++ -O2 -fno-unroll-loops -S -emit-llvm -fenable-ripple \
// RUN:   --target=hexagon -mv68 -mhvx -mhvx-qfloat -ffreestanding \
// RUN:   -Wno-nan-infinity-disabled %s -o - \
// RUN:   | FileCheck %s

#include <ripple.h>

// Model the candidate interface locally to keep this frontend test self-contained.
typedef float f32t32 __attribute__((vector_size(128)));
typedef int i32t32 __attribute__((vector_size(128)));
#define RIPPLE_VECTOR_CANDIDATE __attribute__((ripple_vector_candidate))
#define RIPPLE_CANDIDATE_INLINE                                                 \
  static inline __attribute__((always_inline, unused)) RIPPLE_VECTOR_CANDIDATE

#ifdef __cplusplus
extern "C" {
#endif

float _FSinh(float, float);
float _FCosh(float, float);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_sinhf(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_coshf(f32t32);

#ifdef __cplusplus
}
#endif

// Unit scale selects the vector candidate; other scales retain masked SDK calls.
RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew_mask__FSinh(f32t32 x, float scale,
                                                  i32t32 mask) {
  if (scale == 1.0f)
    return ripple_ew_pure_sinhf(x);
  f32t32 result = {0};
  for (unsigned i = 0; i < sizeof(x) / sizeof(float); ++i)
    if (mask[i])
      result[i] = _FSinh(x[i], scale);
  return result;
}

RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew_mask__FCosh(f32t32 x, float scale,
                                                  i32t32 mask) {
  if (scale == 1.0f)
    return ripple_ew_pure_coshf(x);
  f32t32 result = {0};
  for (unsigned i = 0; i < sizeof(x) / sizeof(float); ++i)
    if (mask[i])
      result[i] = _FCosh(x[i], scale);
  return result;
}

RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew__FSinh(f32t32 x, float scale) {
  i32t32 mask = {0};
  return ripple_ew_mask__FSinh(x, scale, ~mask);
}

RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew__FCosh(f32t32 x, float scale) {
  i32t32 mask = {0};
  return ripple_ew_mask__FCosh(x, scale, ~mask);
}

#ifdef __cplusplus
extern "C" {
#endif

// Model the SDK inlines without requiring an installed SDK.
static inline float sdk_sinhf(float x) { return _FSinh(x, 1.0f); }
static inline float sdk_coshf(float x) { return _FCosh(x, 1.0f); }

// CHECK-LABEL: define{{.*}} @inline_sinh(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinhf(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: ret void
void inline_sinh(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_sinhf(in[i]);
}

// CHECK-LABEL: define{{.*}} @inline_cosh(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_coshf(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: ret void
void inline_cosh(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_coshf(in[i]);
}

// CHECK-LABEL: define{{.*}} @tail_sinh(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinhf(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: ret void
void tail_sinh(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    out[i] = sdk_sinhf(in[i]);
}

// CHECK-LABEL: define{{.*}} @guarded_cosh(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_coshf(
// CHECK-NOT: @_F{{Sinh|Cosh}}(
// CHECK: ret void
void guarded_cosh(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    if (in[i] > 0.0f)
      out[i] = sdk_coshf(in[i]);
}

// CHECK-LABEL: define{{.*}} @varying_sinh(
// CHECK: call{{.*}} @_FSinh(
// CHECK: ret void
void varying_sinh(float *out, const float *in, const float *scale) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSinh(in[i], scale[i]);
}

// CHECK-LABEL: define{{.*}} @scaled_sinh(
// CHECK: call{{.*}} @_FSinh(
// CHECK: ret void
void scaled_sinh(float *out, const float *in, float scale) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSinh(in[i], scale);
}

// CHECK-LABEL: define{{.*}} @constant_scaled_cosh(
// CHECK: call{{.*}} @_FCosh(
// CHECK: ret void
void constant_scaled_cosh(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FCosh(in[i], 0.001f);
}

// CHECK-LABEL: define{{.*}} @scaled_cosh(
// CHECK: call{{.*}} @_FCosh(
// CHECK: ret void
void scaled_cosh(float *out, const float *in, float scale) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FCosh(in[i], scale);
}

// Non-unit scales must not call the SDK helpers on inactive lanes.
// CHECK-LABEL: define{{.*}} @tail_scaled_sinh(
// CHECK: [[SIN_INPUT:%.*]] = {{.*}}call <32 x float> @llvm.masked.load.{{.*}}(ptr {{.*}}, <32 x i1> [[SIN_TAIL:%[^,]+]], <32 x float> poison)
// CHECK: [[SIN_MASK:%.*]] = zext <32 x i1> [[SIN_TAIL]] to <32 x i32>
// CHECK: [[SIN_LANE:%.*]] = extractelement <32 x i32> [[SIN_MASK]], i32 [[SIN_I:%.*]]
// CHECK-NEXT: [[SIN_SKIP:%.*]] = icmp eq i32 [[SIN_LANE]], 0
// CHECK-NEXT: br i1 [[SIN_SKIP]], label %{{.*}}, label %[[SIN_ACTIVE:.*]]
// CHECK: [[SIN_ACTIVE]]:
// CHECK-NEXT: [[SIN_X:%.*]] = extractelement <32 x float> [[SIN_INPUT]], i32 [[SIN_I]]
// CHECK-NEXT: {{%.*}} = {{.*}}call float @_FSinh(float noundef [[SIN_X]], float noundef 1.000000e-03)
// CHECK-NOT: @_FSinh(
// CHECK: ret void
void tail_scaled_sinh(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    out[i] = _FSinh(in[i], 0.001f);
}

// CHECK-LABEL: define{{.*}} @tail_scaled_cosh(
// CHECK: [[COS_INPUT:%.*]] = {{.*}}call <32 x float> @llvm.masked.load.{{.*}}(ptr {{.*}}, <32 x i1> [[COS_TAIL:%[^,]+]], <32 x float> poison)
// CHECK: [[COS_MASK:%.*]] = zext <32 x i1> [[COS_TAIL]] to <32 x i32>
// CHECK: [[COS_LANE:%.*]] = extractelement <32 x i32> [[COS_MASK]], i32 [[COS_I:%.*]]
// CHECK-NEXT: [[COS_SKIP:%.*]] = icmp eq i32 [[COS_LANE]], 0
// CHECK-NEXT: br i1 [[COS_SKIP]], label %{{.*}}, label %[[COS_ACTIVE:.*]]
// CHECK: [[COS_ACTIVE]]:
// CHECK-NEXT: [[COS_X:%.*]] = extractelement <32 x float> [[COS_INPUT]], i32 [[COS_I]]
// CHECK-NEXT: {{%.*}} = {{.*}}call float @_FCosh(float noundef [[COS_X]], float noundef 1.000000e-03)
// CHECK-NOT: @_FCosh(
// CHECK: ret void
void tail_scaled_cosh(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    out[i] = _FCosh(in[i], 0.001f);
}

// CHECK-LABEL: define{{.*}} @guarded_scaled_sinh(
// CHECK: [[GSIN_COND:%.*]] = fcmp ogt <32 x float> {{%.*}}, zeroinitializer
// CHECK: [[GSIN_MASK:%.*]] = zext <32 x i1> [[GSIN_COND]] to <32 x i32>
// CHECK: [[GSIN_LANE:%.*]] = extractelement <32 x i32> [[GSIN_MASK]], i32 {{%.*}}
// CHECK-NEXT: [[GSIN_SKIP:%.*]] = icmp eq i32 [[GSIN_LANE]], 0
// CHECK-NEXT: br i1 [[GSIN_SKIP]], label %{{.*}}, label %[[GSIN_ACTIVE:.*]]
// CHECK: [[GSIN_ACTIVE]]:
// CHECK: call{{.*}} @_FSinh(
// CHECK-NOT: @_FSinh(
// CHECK: ret void
void guarded_scaled_sinh(float *out, const float *in, float scale) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    if (in[i] > 0.0f)
      out[i] = _FSinh(in[i], scale);
}

// CHECK-LABEL: define{{.*}} @guarded_scaled_cosh(
// CHECK: [[GCOS_COND:%.*]] = fcmp ogt <32 x float> {{%.*}}, zeroinitializer
// CHECK: [[GCOS_MASK:%.*]] = zext <32 x i1> [[GCOS_COND]] to <32 x i32>
// CHECK: [[GCOS_LANE:%.*]] = extractelement <32 x i32> [[GCOS_MASK]], i32 {{%.*}}
// CHECK-NEXT: [[GCOS_SKIP:%.*]] = icmp eq i32 [[GCOS_LANE]], 0
// CHECK-NEXT: br i1 [[GCOS_SKIP]], label %{{.*}}, label %[[GCOS_ACTIVE:.*]]
// CHECK: [[GCOS_ACTIVE]]:
// CHECK: call{{.*}} @_FCosh(
// CHECK-NOT: @_FCosh(
// CHECK: ret void
void guarded_scaled_cosh(float *out, const float *in, float scale) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    if (in[i] > 0.0f)
      out[i] = _FCosh(in[i], scale);
}

#ifdef __cplusplus
}
#endif
