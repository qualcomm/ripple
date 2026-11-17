// REQUIRES: hexagon-registered-target
// RUN: %clang -O2 -S -emit-llvm -fenable-ripple --target=hexagon -mv81 -mhvx \
// RUN:   -mhvx-ieee-fp -ffreestanding %s -o - | FileCheck %s
// RUN: %clang -x c++ -O2 -S -emit-llvm -fenable-ripple --target=hexagon -mv81 \
// RUN:   -mhvx -mhvx-ieee-fp -ffreestanding %s -o - | FileCheck %s
// RUN: %clang -O2 -S -emit-llvm -fenable-ripple --target=hexagon -mv68 -mhvx \
// RUN:   -mhvx-qfloat -ffreestanding %s -o - | FileCheck %s
// RUN: %clang -x c++ -O2 -S -emit-llvm -fenable-ripple --target=hexagon -mv68 \
// RUN:   -mhvx -mhvx-qfloat -ffreestanding %s -o - | FileCheck %s

#include <ripple.h>

// Model the candidate interfaces locally; do not depend on compiler-rt or SDK
// headers in this frontend test.
typedef float f32t32 __attribute__((vector_size(128)));
#define RIPPLE_VECTOR_CANDIDATE __attribute__((ripple_vector_candidate))
#define RIPPLE_CANDIDATE_INLINE                                                \
  static inline __attribute__((always_inline, unused)) RIPPLE_VECTOR_CANDIDATE

#ifdef __cplusplus
extern "C" {
#endif
float _FSin(float, unsigned int);
float _FLog(float, int);
float expf(float);
float sqrtf(float);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_sinf(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_cosf(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_logf(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_log10f(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_log2f(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_expf(f32t32);
extern RIPPLE_VECTOR_CANDIDATE f32t32 ripple_ew_pure_sqrtf(f32t32);
#ifdef __cplusplus
}
#endif

RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew_pure__FSin(f32t32 x,
                                                    unsigned int quadrant) {
  f32t32 result = (quadrant & 1) ? ripple_ew_pure_cosf(x)
                                : ripple_ew_pure_sinf(x);
  return (quadrant & 2) ? -result : result;
}

RIPPLE_CANDIDATE_INLINE f32t32 ripple_ew_pure__FLog(f32t32 x, int base) {
  if (base == 0)
    return ripple_ew_pure_logf(x);
  if (base > 0)
    return ripple_ew_pure_log10f(x);
  return ripple_ew_pure_log2f(x);
}

#ifdef __cplusplus
extern "C" {
#endif

// These constant selectors are the calls exposed by the SDK float overloads.
static inline float sdk_sin(float x) { return _FSin(x, 0); }
static inline float sdk_cos(float x) { return _FSin(x, 1); }
static inline float sdk_log(float x) { return _FLog(x, 0); }
static inline float sdk_log10(float x) { return _FLog(x, 1); }
static inline float sdk_log2(float x) { return _FLog(x, -1); }

// CHECK-LABEL: define{{.*}} @inline_sin(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinf(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void inline_sin(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_sin(in[i]);
}

// CHECK-LABEL: define{{.*}} @inline_cos(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_cosf(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void inline_cos(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_cos(in[i]);
}

// CHECK-LABEL: define{{.*}} @inline_log(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_logf(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void inline_log(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_log(in[i]);
}

// CHECK-LABEL: define{{.*}} @inline_log10(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log10f(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void inline_log10(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_log10(in[i]);
}

// CHECK-LABEL: define{{.*}} @inline_log2(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log2f(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void inline_log2(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_log2(in[i]);
}

// CHECK-LABEL: define{{.*}} @tail_cos(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_cosf(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void tail_cos(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    out[i] = sdk_cos(in[i]);
}

// CHECK-LABEL: define{{.*}} @guarded_log(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_logf(
// CHECK-NOT: @_F{{Sin|Log}}(
// CHECK: ret void
void guarded_log(float *out, const float *in, unsigned n) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < n; ++i)
    if (in[i] > 0.0f)
      out[i] = sdk_log(in[i]);
}

// CHECK-LABEL: define{{.*}} @uniform_sin(
// CHECK-NOT: @_FSin(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_cosf(
// CHECK-NOT: @_FSin(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinf(
// CHECK-NOT: @_FSin(
// CHECK: fneg <32 x float>
// CHECK-NOT: @_FSin(
// CHECK: ret void
void uniform_sin(float *out, const float *in, unsigned int quadrant) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSin(in[i], quadrant);
}

// CHECK-LABEL: define{{.*}} @uniform_log(
// CHECK-NOT: @_FLog(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_logf(
// CHECK-NOT: @_FLog(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log10f(
// CHECK-NOT: @_FLog(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log2f(
// CHECK-NOT: @_FLog(
// CHECK: ret void
void uniform_log(float *out, const float *in, int base) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FLog(in[i], base);
}

// CHECK-LABEL: define{{.*}} @varying_sin(
// CHECK: call{{.*}} @_FSin(
// CHECK: ret void
void varying_sin(float *out, const float *in, const unsigned int *quadrant) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSin(in[i], quadrant[i]);
}

// CHECK-LABEL: define{{.*}} @varying_log(
// CHECK: call{{.*}} @_FLog(
// CHECK: ret void
void varying_log(float *out, const float *in, const int *base) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FLog(in[i], base[i]);
}

// Quadrants 2 and 3 negate sine and cosine, respectively.
// CHECK-LABEL: define{{.*}} @negative_sin(
// CHECK-NOT: @_FSin(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinf(
// CHECK: fneg <32 x float>
// CHECK-NOT: @_FSin(
// CHECK: ret void
void negative_sin(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSin(in[i], 2U);
}

// CHECK-LABEL: define{{.*}} @negative_cos(
// CHECK-NOT: @_FSin(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_cosf(
// CHECK: fneg <32 x float>
// CHECK-NOT: @_FSin(
// CHECK: ret void
void negative_cos(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSin(in[i], 3U);
}

// Quadrant offsets repeat modulo four.
// CHECK-LABEL: define{{.*}} @wrapped_sin(
// CHECK-NOT: @_FSin(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sinf(
// CHECK-NOT: fneg
// CHECK-NOT: @_FSin(
// CHECK: ret void
void wrapped_sin(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FSin(in[i], 4U);
}

// Every positive Dinkumware base selector selects log10.
// CHECK-LABEL: define{{.*}} @positive_log(
// CHECK-NOT: @_FLog(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log10f(
// CHECK-NOT: @_FLog(
// CHECK: ret void
void positive_log(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FLog(in[i], 2);
}

// Every negative Dinkumware base selector selects log2.
// CHECK-LABEL: define{{.*}} @negative_log(
// CHECK-NOT: @_FLog(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_log2f(
// CHECK-NOT: @_FLog(
// CHECK: ret void
void negative_log(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = _FLog(in[i], -2);
}

// Other SDK wrappers retain the public math names and need no helper candidate.
static inline float sdk_exp(float x) { return expf(x); }
// CHECK-LABEL: define{{.*}} @inline_exp(
// CHECK-NOT: call{{.*}} float @expf(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_expf(
// CHECK-NOT: call{{.*}} float @expf(
// CHECK: ret void
void inline_exp(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_exp(in[i]);
}

static inline float sdk_sqrt(float x) { return sqrtf(x); }
// CHECK-LABEL: define{{.*}} @inline_sqrt(
// CHECK-NOT: call{{.*}} float @sqrtf(
// CHECK: call{{.*}} <32 x float> @ripple_ew_pure_sqrtf(
// CHECK-NOT: call{{.*}} float @sqrtf(
// CHECK: ret void
void inline_sqrt(float *out, const float *in) {
  ripple_block_t bs = ripple_set_block_shape(0, 32);
  ripple_parallel(bs, 0);
  for (unsigned i = 0; i < 32; ++i)
    out[i] = sdk_sqrt(in[i]);
}

#ifdef __cplusplus
}
#endif
