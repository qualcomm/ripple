; RUN: llc -O0 -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b \
; RUN:   < %s | FileCheck %s

; Ensure both halves of an HvxWR pair are promoted to qf32 before
; V6_vconv_hf_qf32 consumes the pair.

; CHECK: v[[HI:[0-9]+]].qf32 = vadd(v[[HI]].sf,
; CHECK: v[[LO:[0-9]+]].qf32 = vadd(v[[LO]].sf,
; CHECK: .hf = v[[HI]]:[[LO]].qf32

define void @fptrunc_v64f32_pair(ptr %out, ptr %in) #0 {
  %v = load <64 x float>, ptr %in, align 256
  %t = fptrunc <64 x float> %v to <64 x half>
  store <64 x half> %t, ptr %out, align 128
  ret void
}

attributes #0 = { nounwind "target-cpu"="hexagonv79" "target-features"="+hvxv79,+hvx-length128b" }
