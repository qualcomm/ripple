// REQUIRES: hexagon-registered-target || aarch64-registered-target || x86-registered-target
// RUN: %if hexagon-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple hexagon -std=c++17 -fenable-ripple -fsyntax-only -verify %s %}
// RUN: %if aarch64-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple aarch64-unknown-linux-gnu -std=c++17 -fenable-ripple -fsyntax-only -verify %s %}
// RUN: %if x86-registered-target %{ %clang_cc1 -I %S/../../lib/Headers/ripple_include -triple x86_64-unknown-linux-gnu -std=c++17 -fenable-ripple -fsyntax-only -verify %s %}

#include <ripple.h>

// bool is an integral type, but ripple_reinterp excludes it as a destination.
// expected-error@__ripple_vec.h:* {{Expected integral or floating point type to ripple reinterpret}}
void reinterp_bool_to_i8(ripple_block_t block, bool value) {
  ripple_reinterp<bool>(block, value); // expected-note {{in instantiation of function template specialization 'ripple_reinterp<bool, bool>' requested here}}
}
