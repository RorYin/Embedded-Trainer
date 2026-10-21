#include <stdio.h>
#include <stdint.h>

void my_memcpy(uint8_t *dst, const uint8_t *src, uint16_t n)
{
    while (n--) {
        *dst = *src;
        dst++;
        src++;
    }
}

uint32_t bytes_to_u32_pun(const uint8_t *bytes)
{
    uint32_t val[1];
    my_memcpy((uint8_t*)val, bytes, 4);
    return val[0];
}
