#include <stdio.h>
#include <stdint.h>

#define FRAME_SOF 0x55
#define FRAME_EOF 0xAA

void build_command_frame(uint8_t cmd, uint16_t value, uint8_t *out)
{
    out[0] = FRAME_SOF;
    out[1] = cmd;
    out[2] = (uint8_t)(value >> 8);
    out[3] = (uint8_t)(value & 0xFF);
    out[4] = (uint8_t)(out[0] ^ out[1] ^ out[2] ^ out[3]);
    out[5] = FRAME_EOF;
}

int validate_command_frame(const uint8_t *in)
{
    if (in[0] != FRAME_SOF) return 0;
    if (in[5] != FRAME_EOF) return 0;
    uint8_t expected_cs = (uint8_t)(in[0] ^ in[1] ^ in[2] ^ in[3]);
    return in[4] == expected_cs;
}
