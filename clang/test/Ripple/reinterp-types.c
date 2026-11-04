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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_HELPERS
#define REINTERP(DST, BS, X) ripple_reinterp_##DST(BS, X)
#else
#define REINTERP(DST, BS, X) __builtin_ripple_reinterp_##DST(BS, X)
#endif

// Check exact source and result types before Ripple lowering, then check
// runtime input blocks lower to destination stores without numeric conversion.
#ifndef LOWERED
#define PAIR(SRC_CT, SRC, SRC_BITS, DST_CT, DST, DST_BITS)                     \
  DST_CT SRC##_to_##DST(ripple_block_t block, SRC_CT value) {                  \
    return REINTERP(DST, block, value);                                        \
  }
#else
#define PAIR(SRC_CT, SRC, SRC_BITS, DST_CT, DST, DST_BITS)                     \
  void SRC##_to_##DST(const SRC_CT *input, DST_CT *output) {                   \
    ripple_block_t source = ripple_set_block_shape(0, 128 / SRC_BITS);         \
    ripple_block_t destination = ripple_set_block_shape(0, 128 / DST_BITS);    \
    SRC_CT value = input[ripple_id(source, 0)];                                \
    output[ripple_id(destination, 0)] = REINTERP(DST, destination, value);     \
  }
#endif

PAIR(signed char, i8, 8, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @i8_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(signed char, i8, 8, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @i8_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(signed char, i8, 8, short, i16, 16)
// RAW-LABEL: define {{.*}} @i8_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(signed char, i8, 8, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @i8_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(signed char, i8, 8, int, i32, 32)
// RAW-LABEL: define {{.*}} @i8_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(signed char, i8, 8, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @i8_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(signed char, i8, 8, long long, i64, 64)
// RAW-LABEL: define {{.*}} @i8_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(signed char, i8, 8, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @i8_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(signed char, i8, 8, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @i8_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(signed char, i8, 8, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @i8_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(signed char, i8, 8, float, f32, 32)
// RAW-LABEL: define {{.*}} @i8_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(signed char, i8, 8, double, f64, 64)
// RAW-LABEL: define {{.*}} @i8_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @i8_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @u8_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @u8_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, short, i16, 16)
// RAW-LABEL: define {{.*}} @u8_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @u8_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, int, i32, 32)
// RAW-LABEL: define {{.*}} @u8_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @u8_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, long long, i64, 64)
// RAW-LABEL: define {{.*}} @u8_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @u8_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @u8_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @u8_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, float, f32, 32)
// RAW-LABEL: define {{.*}} @u8_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(unsigned char, u8, 8, double, f64, 64)
// RAW-LABEL: define {{.*}} @u8_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i8(ptr {{.*}}, i8 {{.*}})
// LOWERED-LABEL: define {{.*}} @u8_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(short, i16, 16, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @i16_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(short, i16, 16, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @i16_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(short, i16, 16, short, i16, 16)
// RAW-LABEL: define {{.*}} @i16_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(short, i16, 16, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @i16_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(short, i16, 16, int, i32, 32)
// RAW-LABEL: define {{.*}} @i16_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(short, i16, 16, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @i16_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(short, i16, 16, long long, i64, 64)
// RAW-LABEL: define {{.*}} @i16_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(short, i16, 16, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @i16_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(short, i16, 16, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @i16_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(short, i16, 16, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @i16_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(short, i16, 16, float, f32, 32)
// RAW-LABEL: define {{.*}} @i16_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(short, i16, 16, double, f64, 64)
// RAW-LABEL: define {{.*}} @i16_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @i16_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @u16_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @u16_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, short, i16, 16)
// RAW-LABEL: define {{.*}} @u16_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @u16_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, int, i32, 32)
// RAW-LABEL: define {{.*}} @u16_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @u16_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, long long, i64, 64)
// RAW-LABEL: define {{.*}} @u16_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @u16_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @u16_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @u16_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, float, f32, 32)
// RAW-LABEL: define {{.*}} @u16_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(unsigned short, u16, 16, double, f64, 64)
// RAW-LABEL: define {{.*}} @u16_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i16(ptr {{.*}}, i16 {{.*}})
// LOWERED-LABEL: define {{.*}} @u16_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(int, i32, 32, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @i32_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(int, i32, 32, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @i32_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(int, i32, 32, short, i16, 16)
// RAW-LABEL: define {{.*}} @i32_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(int, i32, 32, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @i32_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(int, i32, 32, int, i32, 32)
// RAW-LABEL: define {{.*}} @i32_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(int, i32, 32, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @i32_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(int, i32, 32, long long, i64, 64)
// RAW-LABEL: define {{.*}} @i32_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(int, i32, 32, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @i32_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(int, i32, 32, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @i32_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(int, i32, 32, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @i32_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(int, i32, 32, float, f32, 32)
// RAW-LABEL: define {{.*}} @i32_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(int, i32, 32, double, f64, 64)
// RAW-LABEL: define {{.*}} @i32_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @i32_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @u32_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @u32_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, short, i16, 16)
// RAW-LABEL: define {{.*}} @u32_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @u32_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, int, i32, 32)
// RAW-LABEL: define {{.*}} @u32_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @u32_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, long long, i64, 64)
// RAW-LABEL: define {{.*}} @u32_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @u32_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @u32_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @u32_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, float, f32, 32)
// RAW-LABEL: define {{.*}} @u32_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(unsigned int, u32, 32, double, f64, 64)
// RAW-LABEL: define {{.*}} @u32_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i32(ptr {{.*}}, i32 {{.*}})
// LOWERED-LABEL: define {{.*}} @u32_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(long long, i64, 64, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @i64_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(long long, i64, 64, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @i64_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(long long, i64, 64, short, i16, 16)
// RAW-LABEL: define {{.*}} @i64_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(long long, i64, 64, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @i64_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(long long, i64, 64, int, i32, 32)
// RAW-LABEL: define {{.*}} @i64_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(long long, i64, 64, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @i64_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(long long, i64, 64, long long, i64, 64)
// RAW-LABEL: define {{.*}} @i64_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(long long, i64, 64, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @i64_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(long long, i64, 64, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @i64_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(long long, i64, 64, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @i64_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(long long, i64, 64, float, f32, 32)
// RAW-LABEL: define {{.*}} @i64_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(long long, i64, 64, double, f64, 64)
// RAW-LABEL: define {{.*}} @i64_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @i64_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @u64_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @u64_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, short, i16, 16)
// RAW-LABEL: define {{.*}} @u64_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @u64_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, int, i32, 32)
// RAW-LABEL: define {{.*}} @u64_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @u64_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, long long, i64, 64)
// RAW-LABEL: define {{.*}} @u64_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @u64_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @u64_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @u64_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, float, f32, 32)
// RAW-LABEL: define {{.*}} @u64_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(unsigned long long, u64, 64, double, f64, 64)
// RAW-LABEL: define {{.*}} @u64_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.i64(ptr {{.*}}, i64 {{.*}})
// LOWERED-LABEL: define {{.*}} @u64_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @f16_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @f16_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, short, i16, 16)
// RAW-LABEL: define {{.*}} @f16_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @f16_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, int, i32, 32)
// RAW-LABEL: define {{.*}} @f16_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @f16_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, long long, i64, 64)
// RAW-LABEL: define {{.*}} @f16_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @f16_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @f16_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @f16_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, float, f32, 32)
// RAW-LABEL: define {{.*}} @f16_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(_Float16, f16, 16, double, f64, 64)
// RAW-LABEL: define {{.*}} @f16_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.f16(ptr {{.*}}, half {{.*}})
// LOWERED-LABEL: define {{.*}} @f16_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @bf16_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @bf16_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, short, i16, 16)
// RAW-LABEL: define {{.*}} @bf16_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @bf16_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, int, i32, 32)
// RAW-LABEL: define {{.*}} @bf16_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @bf16_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, long long, i64, 64)
// RAW-LABEL: define {{.*}} @bf16_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @bf16_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @bf16_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @bf16_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, float, f32, 32)
// RAW-LABEL: define {{.*}} @bf16_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(__bf16, bf16, 16, double, f64, 64)
// RAW-LABEL: define {{.*}} @bf16_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.bf16(ptr {{.*}}, bfloat {{.*}})
// LOWERED-LABEL: define {{.*}} @bf16_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(float, f32, 32, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @f32_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(float, f32, 32, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @f32_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(float, f32, 32, short, i16, 16)
// RAW-LABEL: define {{.*}} @f32_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(float, f32, 32, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @f32_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(float, f32, 32, int, i32, 32)
// RAW-LABEL: define {{.*}} @f32_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(float, f32, 32, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @f32_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(float, f32, 32, long long, i64, 64)
// RAW-LABEL: define {{.*}} @f32_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(float, f32, 32, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @f32_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(float, f32, 32, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @f32_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(float, f32, 32, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @f32_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(float, f32, 32, float, f32, 32)
// RAW-LABEL: define {{.*}} @f32_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(float, f32, 32, double, f64, 64)
// RAW-LABEL: define {{.*}} @f32_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.f32(ptr {{.*}}, float {{.*}})
// LOWERED-LABEL: define {{.*}} @f32_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

PAIR(double, f64, 64, signed char, i8, 8)
// RAW-LABEL: define {{.*}} @f64_to_i8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_i8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(double, f64, 64, unsigned char, u8, 8)
// RAW-LABEL: define {{.*}} @f64_to_u8(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_u8(
// LOWERED: store <16 x i8> {{.*}}, ptr %output

PAIR(double, f64, 64, short, i16, 16)
// RAW-LABEL: define {{.*}} @f64_to_i16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_i16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(double, f64, 64, unsigned short, u16, 16)
// RAW-LABEL: define {{.*}} @f64_to_u16(
// RAW: call i16 @llvm.ripple.reinterp.int.i16.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_u16(
// LOWERED: store <8 x i16> {{.*}}, ptr %output

PAIR(double, f64, 64, int, i32, 32)
// RAW-LABEL: define {{.*}} @f64_to_i32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_i32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(double, f64, 64, unsigned int, u32, 32)
// RAW-LABEL: define {{.*}} @f64_to_u32(
// RAW: call i32 @llvm.ripple.reinterp.int.i32.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_u32(
// LOWERED: store <4 x i32> {{.*}}, ptr %output

PAIR(double, f64, 64, long long, i64, 64)
// RAW-LABEL: define {{.*}} @f64_to_i64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_i64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(double, f64, 64, unsigned long long, u64, 64)
// RAW-LABEL: define {{.*}} @f64_to_u64(
// RAW: call i64 @llvm.ripple.reinterp.int.i64.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_u64(
// LOWERED: store <2 x i64> {{.*}}, ptr %output

PAIR(double, f64, 64, _Float16, f16, 16)
// RAW-LABEL: define {{.*}} @f64_to_f16(
// RAW: call half @llvm.ripple.reinterp.float.f16.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_f16(
// LOWERED: store <8 x half> {{.*}}, ptr %output

PAIR(double, f64, 64, __bf16, bf16, 16)
// RAW-LABEL: define {{.*}} @f64_to_bf16(
// RAW: call bfloat @llvm.ripple.reinterp.float.bf16.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_bf16(
// LOWERED: store <8 x bfloat> {{.*}}, ptr %output

PAIR(double, f64, 64, float, f32, 32)
// RAW-LABEL: define {{.*}} @f64_to_f32(
// RAW: call float @llvm.ripple.reinterp.float.f32.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_f32(
// LOWERED: store <4 x float> {{.*}}, ptr %output

PAIR(double, f64, 64, double, f64, 64)
// RAW-LABEL: define {{.*}} @f64_to_f64(
// RAW: call double @llvm.ripple.reinterp.float.f64.f64(ptr {{.*}}, double {{.*}})
// LOWERED-LABEL: define {{.*}} @f64_to_f64(
// LOWERED: store <2 x double> {{.*}}, ptr %output

#undef PAIR
#undef REINTERP

#ifdef __cplusplus
}
#endif
