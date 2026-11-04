// REQUIRES: hexagon-registered-target || x86-registered-target || aarch64-registered-target
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefixes=RAW,RAW-C %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c11 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefixes=RAW,RAW-C %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --check-prefixes=RAW,RAW-CPP %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,GENERIC --implicit-check-not=llvm.ripple %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,LOWERED-CPP,GENERIC --implicit-check-not=llvm.ripple %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,GENERIC --implicit-check-not=llvm.ripple %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,LOWERED-CPP,GENERIC --implicit-check-not=llvm.ripple %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -std=c23 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,HEX --implicit-check-not=llvm.ripple %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -fenable-ripple -x c++ -std=c++17 -DLOWERED -O2 -emit-llvm %s -o - | FileCheck %s --check-prefixes=LOWERED,LOWERED-CPP,HEX --implicit-check-not=llvm.ripple %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -DINVALID -fsyntax-only -verify %s %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -DINVALID -fsyntax-only -verify %s %}

#include <ripple.h>

#if !defined(__cplusplus) && __STDC_VERSION__ < 202311L
#define bool _Bool
#endif

#ifdef __cplusplus
static_assert(__is_same(decltype(__builtin_ripple_reinterp_i1(
                            (ripple_block_t)0, 0)), bool));
static_assert(__is_same(decltype(ripple_reinterp_i1(
                            (ripple_block_t)0, 0)), bool));
#ifdef LOWERED
template <class T> auto unpack_source(ripple_block_t block, T value) {
  return __builtin_ripple_reinterp_i1(block, value);
}

template <class T> auto pack_source(ripple_block_t block, T value) {
  return ripple_reinterp_u16(block, value);
}
#endif
extern "C" {
#else
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(ripple_reinterp_i1((ripple_block_t)0, 0)), bool),
               "i1 destination is bool");
#endif

#ifdef INVALID
void invalid(ripple_block_t block, int *pointer) {
  __builtin_ripple_reinterp_i1(block); // expected-error {{too few arguments to function call, expected 2, have 1}}
  __builtin_ripple_reinterp_i1(block, 1, 2); // expected-error {{too many arguments to function call, expected 2, have 3}}
  __builtin_ripple_reinterp_i1(pointer, 1); // expected-error {{takes a ripple block shape}}
  __builtin_ripple_reinterp_i1(block, pointer); // expected-error {{ripple reinterpretation requires an integer scalar}}
}
#elif !defined(LOWERED)
bool builtin_to_bool(ripple_block_t block, int value) {
  return __builtin_ripple_reinterp_i1(block, value);
}
// RAW-LABEL: define {{.*}} @builtin_to_bool(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i32(ptr {{.*}}, i32 {{.*}})

bool helper_to_bool(ripple_block_t block, float value) {
  return ripple_reinterp_i1(block, value);
}
// RAW-LABEL: define {{.*}} @helper_to_bool(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.f32(ptr {{.*}}, float {{.*}})

unsigned char from_bool(ripple_block_t block, bool value) {
  return ripple_reinterp_u8(block, value);
}
// RAW-LABEL: define {{.*}} @from_bool(
// RAW: call i8 @llvm.ripple.reinterp.int.i8.i1(ptr {{.*}}, i1 {{.*}})

bool bool_identity(ripple_block_t block, bool value) {
  return ripple_reinterp_i1(block, value);
}
// RAW-LABEL: define {{.*}} @bool_identity(
// RAW: call i1 @llvm.ripple.reinterp.int.i1.i1(ptr {{.*}}, i1 {{.*}})

// A comparison is int in C and bool in C++; preserve the expression's type.
float comparison_to_float(ripple_block_t block, int left, int right) {
  return __builtin_ripple_reinterp_f32(block, left == right);
}
// RAW-LABEL: define {{.*}} @comparison_to_float(
// RAW: [[CMP:%.*]] = icmp eq i32
// RAW-C: [[SOURCE:%.*]] = zext i1 [[CMP]] to i32
// RAW-C: call float @llvm.ripple.reinterp.float.f32.i32(ptr {{.*}}, i32 [[SOURCE]])
// RAW-CPP: call float @llvm.ripple.reinterp.float.f32.i1(ptr {{.*}}, i1 [[CMP]])

// Explicit boolean conversion gives a one-bit source in both C and C++.
float cast_comparison_to_float(ripple_block_t block, int left, int right) {
  return __builtin_ripple_reinterp_f32(block, (bool)(left == right));
}
// RAW-LABEL: define {{.*}} @cast_comparison_to_float(
// RAW: call float @llvm.ripple.reinterp.float.f32.i1(ptr {{.*}}, i1 {{.*}})

float bool_comparison_to_float(ripple_block_t block, int left, int right) {
  bool mask = left == right;
  return __builtin_ripple_reinterp_f32(block, mask);
}
// RAW-LABEL: define {{.*}} @bool_comparison_to_float(
// RAW: call float @llvm.ripple.reinterp.float.f32.i1(ptr {{.*}}, i1 {{.*}})
#else
// Observe the unpacked bits as byte lanes, then pack them back into integers.
typedef int IntVec __attribute__((vector_size(16)));
typedef unsigned char ByteVec __attribute__((vector_size(16)));
#ifdef __cplusplus
#define TO_VEC(N, X) ripple_to_vec<N>(X)
#else
#define TO_VEC(N, X) ripple_to_vec(N, X)
#endif
ByteVec unpack_bits(unsigned short value) {
  ripple_block_t bits = ripple_set_block_shape(0, 16);
  bool unpacked = ripple_reinterp_i1(bits, value);
  return TO_VEC(16, (unsigned char)unpacked);
}
// LOWERED-LABEL: define {{.*}} @unpack_bits(
// LOWERED: bitcast i16 {{.*}} to <16 x i1>
// LOWERED: zext <16 x i1> {{.*}} to <16 x i8>

IntVec round_trip(IntVec value) {
  ripple_block_t words = ripple_set_block_shape(0, 4);
  ripple_block_t bits = ripple_set_block_shape(0, 128);
  bool unpacked = ripple_reinterp_i1(bits, vec_to_ripple(words, value));
  return TO_VEC(4, ripple_reinterp_i32(words, unpacked));
}
// LOWERED-LABEL: define {{.*}} @round_trip(
// GENERIC: ret <4 x i32> %value
// HEX: store <4 x i32> %value, ptr %agg.result
// HEX-NEXT: ret void

unsigned short pack_comparison(ByteVec value) {
  ripple_block_t bytes = ripple_set_block_shape(0, 16);
  ripple_block_t word = ripple_set_block_shape(0, 1);
  bool mask = vec_to_ripple(bytes, value) != 0;
  return TO_VEC(1, ripple_reinterp_u16(word, mask));
}
// LOWERED-LABEL: define {{.*}} @pack_comparison(
// LOWERED: icmp ne <16 x i8> {{.*}}, zeroinitializer
// LOWERED: bitcast <16 x i1> {{.*}} to i16
// LOWERED: ret i16

#ifdef __cplusplus
// Instantiate the destination-bool builtin with an integer source, then the
// destination-integer helper with a bool source, and lower their block values.
ByteVec template_unpack_bits(unsigned short value) {
  ripple_block_t bits = ripple_set_block_shape(0, 16);
  bool unpacked = unpack_source(bits, value);
  return TO_VEC(16, (unsigned char)unpacked);
}
// LOWERED-CPP-LABEL: define {{.*}} @template_unpack_bits(
// LOWERED-CPP: [[BITS:%.*]] = bitcast i16 {{.*}} to <16 x i1>
// LOWERED-CPP: zext <16 x i1> [[BITS]] to <16 x i8>

unsigned short template_pack_comparison(ByteVec value) {
  ripple_block_t bytes = ripple_set_block_shape(0, 16);
  ripple_block_t word = ripple_set_block_shape(0, 1);
  bool mask = vec_to_ripple(bytes, value) != 0;
  return TO_VEC(1, pack_source(word, mask));
}
// LOWERED-CPP-LABEL: define {{.*}} @template_pack_comparison(
// LOWERED-CPP: [[MASK:%.*]] = icmp ne <16 x i8> {{.*}}, zeroinitializer
// LOWERED-CPP: [[WORD:%.*]] = bitcast <16 x i1> [[MASK]] to i16
// LOWERED-CPP: ret i16 [[WORD]]
#endif
#endif

#ifdef __cplusplus
}
#endif
