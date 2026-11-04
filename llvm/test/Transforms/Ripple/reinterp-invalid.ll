; RUN: split-file %s %t
; RUN: not opt -passes=ripple -verify-each -disable-output %t/vector-source.ll 2>&1 | FileCheck %s --check-prefix=SOURCE
; RUN: not opt -passes=ripple -verify-each -disable-output %t/pointer-source.ll 2>&1 | FileCheck %s --check-prefix=SOURCE
; RUN: not opt -passes=ripple -verify-each -disable-output %t/aggregate-source.ll 2>&1 | FileCheck %s --check-prefix=SOURCE
; RUN: not opt -passes=ripple -verify-each -disable-output %t/scalable-source.ll 2>&1 | FileCheck %s --check-prefix=SOURCE
; RUN: not llvm-as %t/vector-destination.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=INTEGER
; RUN: not llvm-as %t/float-vector-destination.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=FLOAT
; RUN: not llvm-as %t/pointer-destination.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=FLOAT
; RUN: not llvm-as %t/void-destination.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=INTEGER
; RUN: not llvm-as %t/float-family-integer-result.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=FLOAT
; RUN: not llvm-as %t/integer-family-float-result.ll -o /dev/null 2>&1 | FileCheck %s --check-prefix=INTEGER

; Ripple rejects unsupported sources before shape propagation or cleanup.
; LLVM verifies the scalar destination category from each intrinsic signature.
; SOURCE: Ripple reinterpretation requires a scalar integer or floating-point source type
; INTEGER: expected any integer type
; FLOAT: expected any fp type

;--- vector-source.ll
define i32 @test(<2 x i32> %input) {
  %block = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 2, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %result = call i32 @llvm.ripple.reinterp.int.i32.v2i32(ptr %block, <2 x i32> %input)
  ret i32 %result
}
declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare i32 @llvm.ripple.reinterp.int.i32.v2i32(ptr, <2 x i32>)

;--- pointer-source.ll
define float @test(ptr %input) {
  %block = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %result = call float @llvm.ripple.reinterp.float.f32.p0(ptr %block, ptr %input)
  ret float %result
}
declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare float @llvm.ripple.reinterp.float.f32.p0(ptr, ptr)

;--- aggregate-source.ll
define i64 @test({ i32, i32 } %input) {
  %block = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %result = call i64 @llvm.ripple.reinterp.int.i64.sl_i32i32s(ptr %block, { i32, i32 } %input)
  ret i64 %result
}
declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare i64 @llvm.ripple.reinterp.int.i64.sl_i32i32s(ptr, { i32, i32 })

;--- scalable-source.ll
define half @test(<vscale x 2 x float> %input) {
  %block = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %result = call half @llvm.ripple.reinterp.float.f16.nxv2f32(ptr %block, <vscale x 2 x float> %input)
  ret half %result
}
declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare half @llvm.ripple.reinterp.float.f16.nxv2f32(ptr, <vscale x 2 x float>)

;--- vector-destination.ll
define <2 x i32> @test(ptr %block, i32 %input) {
  %result = call <2 x i32> @llvm.ripple.reinterp.int.v2i32.i32(ptr %block, i32 %input)
  ret <2 x i32> %result
}
declare <2 x i32> @llvm.ripple.reinterp.int.v2i32.i32(ptr, i32)

;--- float-vector-destination.ll
define <2 x float> @test(ptr %block, i32 %input) {
  %result = call <2 x float> @llvm.ripple.reinterp.float.v2f32.i32(ptr %block, i32 %input)
  ret <2 x float> %result
}
declare <2 x float> @llvm.ripple.reinterp.float.v2f32.i32(ptr, i32)

;--- pointer-destination.ll
define ptr @test(ptr %block, i32 %input) {
  %result = call ptr @llvm.ripple.reinterp.float.p0.i32(ptr %block, i32 %input)
  ret ptr %result
}
declare ptr @llvm.ripple.reinterp.float.p0.i32(ptr, i32)

;--- void-destination.ll
define void @test(ptr %block, i32 %input) {
  call void @llvm.ripple.reinterp.int.isVoid.i32(ptr %block, i32 %input)
  ret void
}
declare void @llvm.ripple.reinterp.int.isVoid.i32(ptr, i32)

;--- float-family-integer-result.ll
define i32 @test(ptr %block, i32 %input) {
  %result = call i32 @llvm.ripple.reinterp.float.i32.i32(ptr %block, i32 %input)
  ret i32 %result
}
declare i32 @llvm.ripple.reinterp.float.i32.i32(ptr, i32)

;--- integer-family-float-result.ll
define float @test(ptr %block, i32 %input) {
  %result = call float @llvm.ripple.reinterp.int.f32.i32(ptr %block, i32 %input)
  ret float %result
}
declare float @llvm.ripple.reinterp.int.f32.i32(ptr, i32)
