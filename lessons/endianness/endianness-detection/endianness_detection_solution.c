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
           ((x >> 24) & 0x000000FF);
}
