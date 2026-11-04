// REQUIRES: x86-registered-target
// RUN: not %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -std=c23 -O2 -emit-llvm %s -o /dev/null 2>&1 | FileCheck %s
// RUN: not %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -fenable-ripple -x c++ -std=c++17 -O2 -emit-llvm %s -o /dev/null 2>&1 | FileCheck %s

#include <ripple.h>

// A 16-bit source requires exactly 16 boolean destination lanes.
bool mismatched_shape(unsigned short value) {
  ripple_block_t bits = ripple_set_block_shape(0, 15);
  return ripple_reinterp_i1(bits, value);
}
// CHECK: error: the provided reinterp flat shape do not match the flat shape of the existing tensor provided.
