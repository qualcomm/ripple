// REQUIRES: hexagon-registered-target || x86-registered-target || aarch64-registered-target
// RUN: %if hexagon-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple hexagon -std=c++11 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple hexagon -std=c++14 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if hexagon-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple hexagon -std=c++20 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if x86-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -std=c++11 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if x86-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -std=c++14 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if x86-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -std=c++20 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -std=c++11 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -std=c++14 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -isystem %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -std=c++20 -fenable-ripple -O2 -disable-llvm-passes -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not=warning: %}

#include <ripple.h>

static_assert(__is_same(decltype(ripple_reinterp<bool>((ripple_block_t)0, 0)),
                        bool), "bool destination returns bool");

extern "C" {

bool reinterp_bool_to_bool(ripple_block_t block, bool value) {
  return ripple_reinterp<bool>(block, value);
}
// CHECK-LABEL: define linkonce_odr {{.*}} @_Z15ripple_reinterpIbbE
// CHECK: call i1 @llvm.ripple.reinterp.int.i1.i1(ptr {{.*}}, i1 {{.*}})

bool reinterp_int_to_bool(ripple_block_t block, int value) {
  return ripple_reinterp<bool>(block, value);
}
// CHECK-LABEL: define linkonce_odr {{.*}} @_Z15ripple_reinterpIbiE
// CHECK: call i1 @llvm.ripple.reinterp.int.i1.i32(ptr {{.*}}, i32 {{.*}})

bool reinterp_float_to_bool(ripple_block_t block, float value) {
  return ripple_reinterp<bool>(block, value);
}
// CHECK-LABEL: define linkonce_odr {{.*}} @_Z15ripple_reinterpIbfE
// CHECK: call i1 @llvm.ripple.reinterp.int.i1.f32(ptr {{.*}}, float {{.*}})

bool reinterp_int_to_const_bool(ripple_block_t block, int value) {
  return ripple_reinterp<const bool>(block, value);
}
// CHECK-LABEL: define linkonce_odr {{.*}} @_Z15ripple_reinterpIKbiE
// CHECK: call i1 @llvm.ripple.reinterp.int.i1.i32(ptr {{.*}}, i32 {{.*}})

unsigned char reinterp_bool_to_u8(ripple_block_t block, bool value) {
  return ripple_reinterp<unsigned char>(block, value);
}
// CHECK-LABEL: define linkonce_odr {{.*}} @_Z15ripple_reinterpIhbE
// CHECK: call i8 @llvm.ripple.reinterp.int.i8.i1(ptr {{.*}}, i1 {{.*}})

}
