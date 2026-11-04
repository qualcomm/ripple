// REQUIRES: x86-registered-target
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DUSE_HELPERS -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -fno-signed-char -O2 -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -fno-signed-char -O2 -disable-llvm-passes -emit-llvm -x c++ %s -o - | FileCheck %s --implicit-check-not=sext --implicit-check-not=zext --implicit-check-not=fpext --implicit-check-not=fptrunc
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DINVALID -fsyntax-only -verify -verify-ignore-unexpected=note %s
// RUN: %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -DINVALID -fsyntax-only -verify -verify-ignore-unexpected=note -x c++ %s

#include <ripple.h>

#if !__has_builtin(__builtin_ripple_reinterp_f32)
#error "destination-only reinterpretation builtin is missing"
#endif
#if __has_builtin(__builtin_ripple_reinterp_f32_u64)
#error "source-suffixed reinterpretation builtin must be removed"
#endif

#ifdef __cplusplus
static_assert(__is_same(decltype(__builtin_ripple_reinterp_i8(
                            (ripple_block_t)0, 0)), signed char),
              "i8 destination is always signed");
static_assert(__is_same(decltype(__builtin_ripple_reinterp_u8(
                            (ripple_block_t)0, 0)), unsigned char),
              "u8 destination is always unsigned");
#else
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(__builtin_ripple_reinterp_i8((ripple_block_t)0, 0)),
                   signed char), "i8 destination is always signed");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(__builtin_ripple_reinterp_u8((ripple_block_t)0, 0)),
                   unsigned char), "u8 destination is always unsigned");
#endif

#if defined(__cplusplus) && defined(INVALID)
// Dependent arguments are checked when the template is instantiated.
template <class Source>
auto deferred_source(ripple_block_t block, Source value) {
  return __builtin_ripple_reinterp_f32(block, value); // expected-error {{ripple reinterpretation requires an integer scalar}}
}

template <class Block> auto deferred_block(Block block, int value) {
  return ripple_reinterp_f32(block, value); // expected-error {{takes a ripple block shape}}
}

void invalid_templates(ripple_block_t block, int *pointer) {
  deferred_source(block, pointer); // expected-note {{in instantiation of function template specialization 'deferred_source<int *>' requested here}}
  deferred_block(pointer, 1); // expected-note {{in instantiation of function template specialization 'deferred_block<int *>' requested here}}
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef INVALID
typedef int Vec __attribute__((vector_size(16)));
struct Aggregate { int x; };
void invalid(ripple_block_t block, Vec vector, struct Aggregate aggregate,
             int *pointer, __int128 wide, long double extended) {
  __builtin_ripple_reinterp_f32(block); // expected-error {{too few arguments to function call, expected 2, have 1}}
  __builtin_ripple_reinterp_f32(block, 1, 2); // expected-error {{too many arguments to function call, expected 2, have 3}}
  __builtin_ripple_reinterp_f32(pointer, 1); // expected-error {{takes a ripple block shape}}
  __builtin_ripple_reinterp_f32(block, pointer); // expected-error {{ripple reinterpretation requires an integer scalar}}
  __builtin_ripple_reinterp_f32(block, vector); // expected-error {{ripple reinterpretation requires an integer scalar}}
  __builtin_ripple_reinterp_f32(block, aggregate); // expected-error {{ripple reinterpretation requires an integer scalar}}
  __builtin_ripple_reinterp_f32(block, wide); // expected-error {{ripple reinterpretation requires an integer scalar}}
  __builtin_ripple_reinterp_f32(block, extended); // expected-error {{ripple reinterpretation requires an integer scalar}}
  __builtin_ripple_reinterp_f32(block, 1 == 1); // C has int; C++ has bool.
}
#ifdef __cplusplus
// expected-error@-3 {{ripple reinterpretation requires an integer scalar}}
void explicit_source_type(ripple_block_t block, int value) {
  ripple_reinterp_f32<float>(block, value); // expected-error {{use of undeclared identifier 'ripple_reinterp_f32'}} expected-warning {{use of function template name with no prior declaration in function call with explicit template arguments is a C++20 extension}}
}
#endif

#else
#ifdef USE_HELPERS
#define REINTERP(DST, BS, X) ripple_reinterp_##DST(BS, X)
#else
#define REINTERP(DST, BS, X) __builtin_ripple_reinterp_##DST(BS, X)
#endif

// Helpers must expose the builtin result type unchanged in both languages.
#ifdef __cplusplus
#define CHECK_RESULT(DST)                                                      \
  static_assert(__is_same(                                                     \
      decltype(ripple_reinterp_##DST((ripple_block_t)0, 0)),                   \
      decltype(__builtin_ripple_reinterp_##DST((ripple_block_t)0, 0))))
#else
#define CHECK_RESULT(DST)                                                      \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(                                            \
          __typeof__(ripple_reinterp_##DST((ripple_block_t)0, 0)),             \
          __typeof__(__builtin_ripple_reinterp_##DST((ripple_block_t)0, 0))),  \
      "helper preserves the builtin result type")
#endif
CHECK_RESULT(i8);
CHECK_RESULT(u8);
CHECK_RESULT(i16);
CHECK_RESULT(u16);
CHECK_RESULT(i32);
CHECK_RESULT(u32);
CHECK_RESULT(i64);
CHECK_RESULT(u64);
CHECK_RESULT(f16);
CHECK_RESULT(bf16);
CHECK_RESULT(f32);
CHECK_RESULT(f64);
#undef CHECK_RESULT

// Plain char is distinct from the signed/unsigned char sources in the type
// matrix. Preserve its width without promotions through builtins and helpers,
// including when plain char is unsigned.
float from_char(ripple_block_t block, char value) {
  return REINTERP(f32, block, value);
}
// CHECK-LABEL: define {{.*}} @from_char(
// CHECK: call float @llvm.ripple.reinterp.float.f32.i8(ptr {{.*}}, i8 {{.*}})

// Parenthesize arguments directly at the builtin call, so commas introduced
// by macro expansion remain inside one argument.
#define MACRO_BLOCK (void)0, block
#define MACRO_VALUE (void)0, value
float from_macro(ripple_block_t block, char value) {
  return ripple_reinterp_f32(MACRO_BLOCK, MACRO_VALUE);
}
// CHECK-LABEL: define {{.*}} @from_macro(
// CHECK: call float @llvm.ripple.reinterp.float.f32.i8(ptr {{.*}}, i8 {{.*}})
#undef MACRO_BLOCK
#undef MACRO_VALUE

#endif

#ifdef __cplusplus
}
#endif
