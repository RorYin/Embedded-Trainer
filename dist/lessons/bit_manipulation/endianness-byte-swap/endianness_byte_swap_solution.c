#include <stdio.h>
#include <stdint.h>

int is_little_endian(void)
{
    uint16_t val[1];
    val[0] = 0x0001;
    uint8_t *p = (uint8_t*)val;
    return p[0] == 1;
}

uint16_t swap16(uint16_t x)
{
    return ((x >> 8) | (x << 8)) & 0xFFFF;
}

uint32_t swap32(uint32_t x)
{
    return (((x >>  0) & 0xFF) << 24) |
           (((x >>  8) & 0xFF) << 16) |
           (((x >> 16) & 0xFF) <<  8) |
           (((x >> 24) & 0xFF) <<  0);
}

int main(void)
{
    if (is_little_endian() != 1) { printf("Failed: is_little_endian\n"); return 1; }
    if (swap16(0xAABB) != 0xBBAA) { printf("Failed: swap16\n"); return 2; }
    if (swap32(0xAABBCCDD) != 0xDDCCBBAA) { printf("Failed: swap32\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
