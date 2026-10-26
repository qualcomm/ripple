// REQUIRES: hexagon-registered-target || aarch64-registered-target || x86-registered-target
// RUN: %if hexagon-registered-target %{ not %clangxx -I %S/../../lib/Headers/ripple_include -S -fenable-ripple -fno-discard-value-names -Rpass-missed=ripple --target=hexagon -O2 -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not="warning:" --implicit-check-not="remark:" %}
// RUN: %if aarch64-registered-target %{ not %clangxx -I %S/../../lib/Headers/ripple_include -S -fenable-ripple -fno-discard-value-names -Rpass-missed=ripple --target=aarch64-unknown-linux-gnu -O2 -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not="warning:" --implicit-check-not="remark:" %}
// RUN: %if x86-registered-target %{ not %clangxx -I %S/../../lib/Headers/ripple_include -S -fenable-ripple -fno-discard-value-names -Rpass-missed=ripple --target=x86_64-unknown-linux-gnu -O2 -emit-llvm %s -o - 2>&1 | FileCheck %s --implicit-check-not="warning:" --implicit-check-not="remark:" %}

#include <ripple.h>
#include <ripple_math.h>

#include "ripple_test.h"

// bool is neither integral (ripple_reinterp explicitly excludes it) nor
// floating point, so this is expected to fail to compile.
void reinterp_bool_to_i8(const bool *input, int8_t *output) {
  auto BS_src = ripple_set_block_shape(0, 32);
  size_t v0 = ripple_id(BS_src, 0);
  bool val = input[v0];

  auto BS_dst = ripple_set_block_shape(0, 32);

  size_t v0_dst = ripple_id(BS_dst, 0);
  int8_t result = ripple_reinterp<bool>(BS_dst, val);
  result = result * 2;
  output[v0_dst] = result;

  // CHECK: error: static assertion failed due to requirement{{.*}}: Expected integral or floating point type to ripple reinterpret
  // CHECK: note: in instantiation of function template specialization 'ripple_reinterp<bool, bool>' requested here
}
