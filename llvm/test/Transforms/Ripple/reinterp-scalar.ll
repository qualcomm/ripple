; RUN: opt -passes='ripple,instcombine,globaldce' -verify-each -S %s | FileCheck %s --implicit-check-not=llvm.ripple

; Packing a vector-shaped source into a one-lane destination must produce a
; scalar value that can be returned directly, rather than a one-element vector.

define i32 @pack_bytes(<4 x i8> %input) {
; CHECK-LABEL: define i32 @pack_bytes(
; CHECK: [[WORD:%.*]] = bitcast <4 x i8> %input to i32
; CHECK: ret i32 [[WORD]]
  %source = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 4, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %destination = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %value = call i8 @llvm.ripple.from.fixed.vector.v4i8(ptr %source, <4 x i8> %input)
  %word = call i32 @llvm.ripple.reinterp.int.i32.i8(ptr %destination, i8 %value)
  ret i32 %word
}

define float @pack_shorts(<2 x i16> %input) {
; CHECK-LABEL: define float @pack_shorts(
; CHECK: [[FLOAT:%.*]] = bitcast <2 x i16> %input to float
; CHECK: ret float [[FLOAT]]
  %source = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 2, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %destination = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %value = call i16 @llvm.ripple.from.fixed.vector.v2i16(ptr %source, <2 x i16> %input)
  %float = call float @llvm.ripple.reinterp.float.f32.i16(ptr %destination, i16 %value)
  ret float %float
}

define float @scalar_bits(i32 %input) {
; CHECK-LABEL: define float @scalar_bits(
; CHECK: [[BITS:%.*]] = bitcast i32 %input to float
; CHECK: ret float [[BITS]]
  %destination = call ptr @llvm.ripple.block.setshape.i64(i64 0, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1)
  %bits = call float @llvm.ripple.reinterp.float.f32.i32(ptr %destination, i32 %input)
  ret float %bits
}

declare ptr @llvm.ripple.block.setshape.i64(i64 immarg, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64)
declare i8 @llvm.ripple.from.fixed.vector.v4i8(ptr, <4 x i8>)
declare i16 @llvm.ripple.from.fixed.vector.v2i16(ptr, <2 x i16>)
declare i32 @llvm.ripple.reinterp.int.i32.i8(ptr, i8)
declare float @llvm.ripple.reinterp.float.f32.i16(ptr, i16)
declare float @llvm.ripple.reinterp.float.f32.i32(ptr, i32)
