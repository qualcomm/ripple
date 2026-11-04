; RUN: opt -passes='ripple,instcombine,globaldce' -verify-each -S %s | FileCheck %s --implicit-check-not=llvm.ripple

; Integer reinterpretation overloads on the return type as well as the source.
; Exercise i7 in both directions without relying on a Clang builtin spelling.
; Seven byte lanes and eight i7 lanes both contain exactly 56 bits.

define <8 x i7> @i8_to_i7(<7 x i8> %input) {
; CHECK-LABEL: define <8 x i7> @i8_to_i7(
; CHECK: [[BITS:%.*]] = bitcast <7 x i8> %input to <8 x i7>
; CHECK: ret <8 x i7> [[BITS]]
  %source = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 7, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %destination = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 8, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %value = call i8 @llvm.ripple.from.fixed.vector.v7i8(ptr %source, <7 x i8> %input)
  %bits = call i7 @llvm.ripple.reinterp.int.i7.i8(ptr %destination, i8 %value)
  %output = call <8 x i7> @llvm.ripple.to.fixed.vector.v8i7(i7 %bits)
  ret <8 x i7> %output
}

define <7 x i8> @i7_to_i8(<8 x i7> %input) {
; CHECK-LABEL: define <7 x i8> @i7_to_i8(
; CHECK: [[BITS:%.*]] = bitcast <8 x i7> %input to <7 x i8>
; CHECK: ret <7 x i8> [[BITS]]
  %source = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 8, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %destination = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 7, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %value = call i7 @llvm.ripple.from.fixed.vector.v8i7(ptr %source, <8 x i7> %input)
  %bits = call i8 @llvm.ripple.reinterp.int.i8.i7(ptr %destination, i7 %value)
  %output = call <7 x i8> @llvm.ripple.to.fixed.vector.v7i8(i8 %bits)
  ret <7 x i8> %output
}

define <8 x i7> @i7_identity(<8 x i7> %input) {
; CHECK-LABEL: define <8 x i7> @i7_identity(
; CHECK: ret <8 x i7> %input
  %block = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 8, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %value = call i7 @llvm.ripple.from.fixed.vector.v8i7(ptr %block, <8 x i7> %input)
  %bits = call i7 @llvm.ripple.reinterp.int.i7.i7(ptr %block, i7 %value)
  %output = call <8 x i7> @llvm.ripple.to.fixed.vector.v8i7(i7 %bits)
  ret <8 x i7> %output
}

declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare i8 @llvm.ripple.from.fixed.vector.v7i8(ptr, <7 x i8>)
declare i7 @llvm.ripple.from.fixed.vector.v8i7(ptr, <8 x i7>)
declare <7 x i8> @llvm.ripple.to.fixed.vector.v7i8(i8)
declare <8 x i7> @llvm.ripple.to.fixed.vector.v8i7(i7)
declare i7 @llvm.ripple.reinterp.int.i7.i8(ptr, i8)
declare i8 @llvm.ripple.reinterp.int.i8.i7(ptr, i7)
declare i7 @llvm.ripple.reinterp.int.i7.i7(ptr, i7)
