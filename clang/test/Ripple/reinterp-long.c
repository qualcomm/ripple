// REQUIRES: hexagon-registered-target || x86-registered-target || aarch64-registered-target
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=LONG32 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=LONG32 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=LONG64 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=LONG64 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=LONG64 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=LONG64 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-pc-windows-msvc -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=LONG32 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-pc-windows-msvc -fenable-ripple -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=LONG32 --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=trunc %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=HEX --implicit-check-not=llvm.ripple --implicit-check-not=alloca %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -DLOWERED -O2 -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=HEX --implicit-check-not=llvm.ripple --implicit-check-not=alloca %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=GENERIC --implicit-check-not=llvm.ripple --implicit-check-not=alloca %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DLOWERED -O2 -emit-llvm -x c++ %s -o - | FileCheck %s --check-prefix=GENERIC --implicit-check-not=llvm.ripple --implicit-check-not=alloca %}

#include <ripple.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOWERED
// long and unsigned long must select their actual source width, without
// numeric conversions, for both floating-point and integer destinations.
float signed_long_to_float(ripple_block_t block, long value) {
  return ripple_reinterp_f32(block, value);
}
// LONG32-LABEL: define {{.*}} @signed_long_to_float(
// LONG32: call float @llvm.ripple.reinterp.float.f32.i32(ptr {{.*}}, i32 {{.*}})
// LONG64-LABEL: define {{.*}} @signed_long_to_float(
// LONG64: call float @llvm.ripple.reinterp.float.f32.i64(ptr {{.*}}, i64 {{.*}})

double unsigned_long_to_double(ripple_block_t block, unsigned long value) {
  return ripple_reinterp_f64(block, value);
}
// LONG32-LABEL: define {{.*}} @unsigned_long_to_double(
// LONG32: call double @llvm.ripple.reinterp.float.f64.i32(ptr {{.*}}, i32 {{.*}})
// LONG64-LABEL: define {{.*}} @unsigned_long_to_double(
// LONG64: call double @llvm.ripple.reinterp.float.f64.i64(ptr {{.*}}, i64 {{.*}})

short signed_long_to_short(ripple_block_t block, long value) {
  return ripple_reinterp_i16(block, value);
}
// LONG32-LABEL: define {{.*}} @signed_long_to_short(
// LONG32: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LONG64-LABEL: define {{.*}} @signed_long_to_short(
// LONG64: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})

unsigned short unsigned_long_to_short(ripple_block_t block,
                                      unsigned long value) {
  return ripple_reinterp_u16(block, value);
}
// LONG32-LABEL: define {{.*}} @unsigned_long_to_short(
// LONG32: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LONG64-LABEL: define {{.*}} @unsigned_long_to_short(
// LONG64: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})

#else
typedef long LongVec __attribute__((vector_size(16)));
typedef unsigned long ULongVec __attribute__((vector_size(16)));
typedef short ShortVec __attribute__((vector_size(16)));

#ifdef __cplusplus
#define TO_VEC(x) ripple_to_vec<8>(x)
#else
#define TO_VEC(x) ripple_to_vec(8, x)
#endif

ShortVec signed_long_vector(LongVec value) {
  ripple_block_t source = ripple_set_block_shape(0, 16 / sizeof(long));
  ripple_block_t destination = ripple_set_block_shape(0, 8);
  return TO_VEC(ripple_reinterp_i16(destination, vec_to_ripple(source, value)));
}
// HEX-LABEL: define {{.*}} @signed_long_vector(
// HEX: store <4 x i32> %value, ptr %agg.result
// HEX-NEXT: ret void
// GENERIC-LABEL: define {{.*}} @signed_long_vector(
// GENERIC: [[SIGNED:%.*]] = bitcast <2 x i64> %value to <8 x i16>
// GENERIC-NEXT: ret <8 x i16> [[SIGNED]]

ShortVec unsigned_long_vector(ULongVec value) {
  ripple_block_t source = ripple_set_block_shape(0, 16 / sizeof(unsigned long));
  ripple_block_t destination = ripple_set_block_shape(0, 8);
  return TO_VEC(ripple_reinterp_i16(destination, vec_to_ripple(source, value)));
}
// HEX-LABEL: define {{.*}} @unsigned_long_vector(
// HEX: store <4 x i32> %value, ptr %agg.result
// HEX-NEXT: ret void
// GENERIC-LABEL: define {{.*}} @unsigned_long_vector(
// GENERIC: [[UNSIGNED:%.*]] = bitcast <2 x i64> %value to <8 x i16>
// GENERIC-NEXT: ret <8 x i16> [[UNSIGNED]]
#endif

#ifdef __cplusplus
}
#endif
