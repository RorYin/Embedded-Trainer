#include <stdio.h>
#include <stdint.h>

int system_is_little_endian(void)
{
    uint16_t val[1];
    val[0] = 0x0001;
    uint8_t *p = (uint8_t*)val;
    return p[0] == 1;
}

uint16_t bswap16(uint16_t x)
{
    return ((x >> 8) | (x << 8)) & 0xFFFF;
}

uint32_t bswap32(uint32_t x)
{
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) <<  8) |
           ((x & 0x00FF0000) >>  8) |
           ((x & 0xFF000000) >> 24);
}

int main(void)
{
    if (system_is_little_endian() != 1) { printf("Failed: endianness check\n"); return 1; }
    if (bswap16(0x1234) != 0x3412) { printf("Failed: bswap16\n"); return 2; }
    if (bswap32(0x12345678) != 0x78563412) { printf("Failed: bswap32\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
