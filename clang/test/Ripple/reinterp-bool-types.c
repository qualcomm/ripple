// REQUIRES: hexagon-registered-target || x86-registered-target || aarch64-registered-target
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c11 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c11 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c11 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c11 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c11 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c11 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c23 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c23 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -x c++ -std=c++17 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -x c++ -std=c++17 -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefix=RAW %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -target-cpu hexagonv81 -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefix=LOWERED --implicit-check-not=llvm.ripple --implicit-check-not=fpext --implicit-check-not=fptrunc --implicit-check-not=sitofp --implicit-check-not=uitofp --implicit-check-not=fptosi --implicit-check-not=fptoui %}

// Hexagon v81 enables both _Float16 and __bf16, so the full type matrix
// is checked here as well as on X86 and AArch64.

#include <ripple.h>

#if !defined(__cplusplus) && __STDC_VERSION__ < 202311L
#define bool _Bool
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_HELPERS
#define REINTERP(DST, BS, X) ripple_reinterp_##DST(BS, X)
#else
#define REINTERP(DST, BS, X) __builtin_ripple_reinterp_##DST(BS, X)
#endif

// Check exact source and result types before Ripple lowering, then check
// runtime input blocks lower without numeric conversion. InstCombine folds
// reinterpretation bitcasts into loads/stores of packed <64 x i1> values.
// Boolean objects in memory still use one byte per element.
#ifndef LOWERED
#define PAIR(SRC_CT, SRC, SRC_BITS, DST_CT, DST, DST_BITS)                     \
  DST_CT SRC##_to_##DST(ripple_block_t block, SRC_CT value) {                  \
    return REINTERP(DST, block, value);                                        \
  }
#else
#define PAIR(SRC_CT, SRC, SRC_BITS, DST_CT, DST, DST_BITS)                     \
  void SRC##_to_##DST(const SRC_CT *input, DST_CT *output) {                   \
    ripple_block_t source = ripple_set_block_shape(0, 64 / SRC_BITS);          \
    ripple_block_t destination = ripple_set_block_shape(0, 64 / DST_BITS);     \
    SRC_CT value = input[ripple_id(source, 0)];                                \
    output[ripple_id(destination, 0)] = REINTERP(DST, destination, value);     \
  }
#endif

PAIR(bool, i1, 1, bool, i1, 1)
// RAW-LABEL: define {{.*}} @i1_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_i1(
// LOWERED: store <64 x i8> {{.*}}, ptr %output

PAIR(bool, i1, 1, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @i1_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_i8(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @i1_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_u8(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, short, i16, 16)
// RAW-LABEL: define {{.*}} @i1_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_i16(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @i1_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_u16(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, int, i32, 32)
// RAW-LABEL: define {{.*}} @i1_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_i32(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @i1_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_u32(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, long long, i64, 64)
// RAW-LABEL: define {{.*}} @i1_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_i64(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @i1_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_u64(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @i1_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_f16(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @i1_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_bf16(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, float, f32, 32)
// RAW-LABEL: define {{.*}} @i1_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_f32(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(bool, i1, 1, double, f64, 64)
// RAW-LABEL: define {{.*}} @i1_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i1(ptr {{.*}}, i1 {{.*}})
// LOWERED-LABEL: define {{.*}} @i1_to_f64(
// LOWERED: [[BYTES:%.*]] = load <64 x i8>, ptr %input
// LOWERED: [[BITS:%.*]] = trunc <64 x i8> [[BYTES]] to <64 x i1>
// LOWERED: store <64 x i1> [[BITS]], ptr %output

PAIR(signed char, i8, 8, bool, i1, 1)
// RAW-LABEL: define {{.*}} @i8_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(unsigned char, u8, 8, bool, i1, 1)
// RAW-LABEL: define {{.*}} @u8_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(short, i16, 16, bool, i1, 1)
// RAW-LABEL: define {{.*}} @i16_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(unsigned short, u16, 16, bool, i1, 1)
// RAW-LABEL: define {{.*}} @u16_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(int, i32, 32, bool, i1, 1)
// RAW-LABEL: define {{.*}} @i32_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(unsigned int, u32, 32, bool, i1, 1)
// RAW-LABEL: define {{.*}} @u32_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(long long, i64, 64, bool, i1, 1)
// RAW-LABEL: define {{.*}} @i64_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(unsigned long long, u64, 64, bool, i1, 1)
// RAW-LABEL: define {{.*}} @u64_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(_Float16, f16, 16, bool, i1, 1)
// RAW-LABEL: define {{.*}} @f16_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(__bf16, bf16, 16, bool, i1, 1)
// RAW-LABEL: define {{.*}} @bf16_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(float, f32, 32, bool, i1, 1)
// RAW-LABEL: define {{.*}} @f32_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

PAIR(double, f64, 64, bool, i1, 1)
// RAW-LABEL: define {{.*}} @f64_to_i1(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_i1(
// LOWERED: [[BITS:%.*]] = load <64 x i1>, ptr %input
// LOWERED: [[BYTES:%.*]] = zext <64 x i1> [[BITS]] to <64 x i8>
// LOWERED: store <64 x i8> [[BYTES]], ptr %output

#undef PAIR
#undef REINTERP

#ifdef __cplusplus
}
#endif
