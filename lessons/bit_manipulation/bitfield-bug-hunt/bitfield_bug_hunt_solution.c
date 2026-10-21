#include <stdio.h>
#include <stdint.h>

uint16_t set_field_fixed(uint16_t reg, uint8_t val)
{
    uint16_t mask = 0x00F0;
    reg &= ~mask;
    reg |= (uint16_t)((val & 0x0F) << 4);
    return reg;
}
