#include <stdio.h>
#include <stdint.h>

uint8_t count_set_bits(uint32_t x)
{
    uint8_t count = 0;
    while (x) {
        x &= (x - 1);
        count++;
    }
    return count;
}

int8_t find_highest_set_bit(uint32_t x)
{
    if (x == 0) return -1;
    for (int i = 31; i >= 0; i--) {
        if ((x >> i) & 1) return (int8_t)i;
    }
    return -1;
}

uint32_t reverse_bits(uint32_t x)
{
    uint32_t res = 0;
    for (int i = 0; i < 32; i++) {
        res = (res << 1) | (x & 1);
        x >>= 1;
    }
    return res;
}
