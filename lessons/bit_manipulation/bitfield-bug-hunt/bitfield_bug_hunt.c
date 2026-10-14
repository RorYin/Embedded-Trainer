#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Bitfield Insertion
 */

uint16_t set_field_fixed(uint16_t reg, uint8_t val)
{
    /* TODO: Fix both bugs and return the updated register */
    (void)reg; (void)val;
    return 0;
}

int main(void)
{
    /* Test 1: Preserves existing bits [15:8] and [3:0] */
    uint16_t reg = 0xF00F;
    uint16_t result = set_field_fixed(reg, 0x5);
    if (result != 0xF05F) {
        printf("Failed: expected 0xF05F, got 0x%x\n", result);
        return 1;
    }

    /* Test 2: Overwrites previous field cleanly */
    reg = 0x12F4;
    result = set_field_fixed(reg, 0x0);
    if (result != 0x1204) {
        printf("Failed: expected 0x1204, got 0x%x\n", result);
        return 2;
    }

    /* Test 3: Value exceeding 4 bits is masked, preventing overflow */
    reg = 0x0000;
    result = set_field_fixed(reg, 0xFF);
    if (result != 0x00F0) {
        printf("Failed: expected 0x00F0, got 0x%x\n", result);
        return 3;
    }

    printf("All tests passed!\n");
    return 0;
}
