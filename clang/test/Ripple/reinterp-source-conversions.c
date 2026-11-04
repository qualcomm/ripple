// REQUIRES: x86-registered-target
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c11 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW-C
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW-C
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW-CPP
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW-CPP
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -DUSE_HELPERS -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple

#include <ripple.h>

#ifdef USE_HELPERS
#define REINTERP(DST, BS, X) ripple_reinterp_##DST(BS, X)
#else
#define REINTERP(DST, BS, X) __builtin_ripple_reinterp_##DST(BS, X)
#endif

#ifdef __cplusplus
// Recheck a dependent source at instantiation, retaining its narrow type and
// deducing the destination type as the wrapper's return type.
template <class T> auto reinterpret_source(ripple_block_t block, T value) {
  return REINTERP(f32, block, value);
}

template <class T>
using reinterpret_result = decltype(REINTERP(i16, (ripple_block_t)0, T{}));
static_assert(__is_same(reinterpret_result<int>, short));
static_assert(__is_same(reinterpret_result<unsigned short>, short));
static_assert(__is_same(decltype(reinterpret_source((ripple_block_t)0,
                                                  short{})), float));

#ifndef LOWERED
extern "C" float template_source(ripple_block_t block, short value) {
  return reinterpret_source(block, value);
}
// RAW-CPP-LABEL: define linkonce_odr {{.*}} @_Z{{.*}}reinterpret_source
// RAW-CPP: call float @llvm.ripple.reinterp.float.f32.i16(ptr {{.*}}, i16 {{.*}})
#else
extern "C" void template_block(const short *input, float *output) {
  ripple_block_t source = ripple_set_block_shape(0, 8);
  ripple_block_t destination = ripple_set_block_shape(0, 4);
  short value = input[ripple_id(source, 0)];
  output[ripple_id(destination, 0)] = reinterpret_source(destination, value);
}
// LOWERED-LABEL: define {{.*}} @template_block(
// LOWERED: [[BITS:%.*]] = load <4 x float>, ptr %input
// LOWERED: store <4 x float> [[BITS]], ptr %output
#endif

#else
// Lvalue conversion must perform an atomic load and expose its unqualified
// value type to source inference without promoting the narrow integer.
float atomic_source(ripple_block_t block, _Atomic(unsigned short) *input) {
  return REINTERP(f32, block, *input);
}
// RAW-C-LABEL: define {{.*}} @atomic_source(
// RAW-C: [[VALUE:%.*]] = load atomic i16, ptr {{.*}} seq_cst
// RAW-C: call float @llvm.ripple.reinterp.float.f32.i16(ptr {{.*}}, i16 [[VALUE]])
#endif
