#include <stdio.h>
#include <stdint.h>

uint16_t set_field_fixed(uint16_t reg, uint8_t val)
{
    uint16_t mask = 0x00F0;
    reg &= ~mask;
    reg |= (uint16_t)((val & 0x0F) << 4);
    return reg;
}

int main(void)
{
    uint16_t reg = 0xF00F;
    uint16_t result = set_field_fixed(reg, 0x5);
    if (result != 0xF05F) {
        printf("Failed: expected 0xF05F, got 0x%x\n", result);
        return 1;
    }

    reg = 0x12F4;
    result = set_field_fixed(reg, 0x0);
    if (result != 0x1204) {
        printf("Failed: expected 0x1204, got 0x%x\n", result);
        return 2;
    }

    reg = 0x0000;
    result = set_field_fixed(reg, 0xFF);
    if (result != 0x00F0) {
        printf("Failed: expected 0x00F0, got 0x%x\n", result);
        return 3;
    }

    printf("All tests passed!\n");
    return 0;
}
