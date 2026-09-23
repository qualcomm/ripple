#pragma once
#include <ripple.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool verify_ripple_sme_api_fixed_vscale();

int32_t zeroAccumulator_i32(ripple_block_t) __arm_streaming __arm_out("za");
float zeroAccumulator_f32(ripple_block_t) __arm_streaming __arm_out("za");

void addHorizToAccumulator_i32(int32_t) __arm_streaming __arm_inout("za");

int32_t getAccumHorizSlice_i32(ripple_block_t,
                               uint32_t) __arm_streaming __arm_out("za");
float getAccumHorizSlice_f32(ripple_block_t,
                             uint32_t) __arm_streaming __arm_out("za");

void outerProductAccumulate_i8(int8_t,
                               int8_t) __arm_streaming __arm_inout("za");
void outerProductAccumulate_f32(float, float) __arm_streaming __arm_inout("za");

#ifdef __cplusplus
}
#endif
