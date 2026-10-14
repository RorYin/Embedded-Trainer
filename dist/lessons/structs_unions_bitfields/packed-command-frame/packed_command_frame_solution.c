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

int main(void)
{
    uint8_t frame[6];
    build_command_frame(0x01, 0x1234, frame);

    if (frame[0] != FRAME_SOF) { printf("Failed: SOF\n"); return 1; }
    if (frame[1] != 0x01) { printf("Failed: CMD\n"); return 2; }
    if (frame[2] != 0x12) { printf("Failed: MSB value\n"); return 3; }
    if (frame[3] != 0x34) { printf("Failed: LSB value\n"); return 4; }
    if (frame[4] != (0x55 ^ 0x01 ^ 0x12 ^ 0x34)) { printf("Failed: checksum\n"); return 5; }
    if (frame[5] != FRAME_EOF) { printf("Failed: EOF\n"); return 6; }

    if (validate_command_frame(frame) != 1) { printf("Failed: validation\n"); return 7; }

    /* Corrupt checksum */
    frame[4] ^= 0xFF;
    if (validate_command_frame(frame) != 0) { printf("Failed: corrupted checksum rejection\n"); return 8; }

    printf("All tests passed!\n");
    return 0;
}
