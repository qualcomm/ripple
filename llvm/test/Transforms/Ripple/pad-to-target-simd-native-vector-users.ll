; REQUIRES: hexagon-registered-target
; RUN: opt -S -passes='module(ripple)' -ripple-pad-to-target-simd -verify-each %s -o - | FileCheck %s

target triple = "hexagon"

; A helper can combine Ripple tensor work with scalar lane extraction from
; its native vector argument, as the SDK hyperbolic candidates do. Padding
; must preserve the native operand's width instead of querying its tensor shape.
define <4 x i32> @native_argument_extract(<4 x i32> %v, i32 %lane) #0 {
; CHECK-LABEL: define <4 x i32> @native_argument_extract(
; CHECK: [[LANE:%.*]] = extractelement <4 x i32> %v, i32 %lane
; CHECK: insertelement <32 x i32> poison, i32 [[LANE]], i64 0
; CHECK: add <32 x i32>
; CHECK: ret <4 x i32>
  %bs = call ptr @llvm.ripple.block.setshape.i32(i32 0, i32 4, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1)
  %x = call i32 @llvm.ripple.from.fixed.vector.v4i32(ptr %bs, <4 x i32> %v)
  %lane_value = extractelement <4 x i32> %v, i32 %lane
  %sum = add i32 %x, %lane_value
  %r = call <4 x i32> @llvm.ripple.to.fixed.vector.v4i32(i32 %sum)
  ret <4 x i32> %r
}

; Vector constants also have no Ripple tensor shape. Keep the dynamic
; extraction scalar even while its result is broadcast into the tensor sum.
define <4 x i32> @native_constant_extract(<4 x i32> %v, i32 %lane) #0 {
; CHECK-LABEL: define <4 x i32> @native_constant_extract(
; CHECK: [[LANE:%.*]] = extractelement <4 x i32> <i32 1, i32 2, i32 3, i32 4>, i32 %lane
; CHECK: insertelement <32 x i32> poison, i32 [[LANE]], i64 0
; CHECK: add <32 x i32>
; CHECK: ret <4 x i32>
  %bs = call ptr @llvm.ripple.block.setshape.i32(i32 0, i32 4, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1)
  %x = call i32 @llvm.ripple.from.fixed.vector.v4i32(ptr %bs, <4 x i32> %v)
  %lane_value = extractelement <4 x i32> <i32 1, i32 2, i32 3, i32 4>, i32 %lane
  %sum = add i32 %x, %lane_value
  %r = call <4 x i32> @llvm.ripple.to.fixed.vector.v4i32(i32 %sum)
  ret <4 x i32> %r
}

; Scalar vector reductions need the same handling for native arguments.
define <4 x i32> @native_argument_reduce(<4 x i32> %v) #0 {
; CHECK-LABEL: define <4 x i32> @native_argument_reduce(
; CHECK: [[REDUCE:%.*]] = call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %v)
; CHECK: insertelement <32 x i32> poison, i32 [[REDUCE]], i64 0
; CHECK: add <32 x i32>
; CHECK: ret <4 x i32>
  %bs = call ptr @llvm.ripple.block.setshape.i32(i32 0, i32 4, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1)
  %x = call i32 @llvm.ripple.from.fixed.vector.v4i32(ptr %bs, <4 x i32> %v)
  %sum_native = call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %v)
  %sum = add i32 %x, %sum_native
  %r = call <4 x i32> @llvm.ripple.to.fixed.vector.v4i32(i32 %sum)
  ret <4 x i32> %r
}

declare ptr @llvm.ripple.block.setshape.i32(i32 immarg, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32)
declare i32 @llvm.ripple.from.fixed.vector.v4i32(ptr, <4 x i32>)
declare <4 x i32> @llvm.ripple.to.fixed.vector.v4i32(i32)
declare i32 @llvm.vector.reduce.add.v4i32(<4 x i32>)

attributes #0 = { "target-cpu"="hexagonv81" "target-features"="+hvx-length128b,+hvxv81,+v81" }
