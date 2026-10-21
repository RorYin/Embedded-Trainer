#include <stdio.h>
#include <stdint.h>

void build_header_fixed(uint8_t *out, uint8_t type, uint16_t payload_len, uint32_t seq)
{
    out[0] = type;
    out[1] = (uint8_t)(payload_len >> 8);
    out[2] = (uint8_t)(payload_len & 0xFF);

    out[3] = (uint8_t)(seq >> 24);
    out[4] = (uint8_t)(seq >> 16);
    out[5] = (uint8_t)(seq >> 8);
    out[6] = (uint8_t)(seq & 0xFF);
}
