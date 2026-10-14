#include <stdio.h>
#include <stdint.h>

/*
 * TASK 5: Command Frame Serialization & Checksum Validation
 *
 * Frame Format (6 bytes):
 *   Byte 0    : SOF (0x55)
 *   Byte 1    : Command code
 *   Bytes 2..3: 16-bit value (Big Endian)
 *   Byte 4    : Checksum (XOR of bytes 0..3)
 *   Byte 5    : EOF (0xAA)
 */

#define FRAME_SOF 0x55
#define FRAME_EOF 0xAA

void build_command_frame(uint8_t cmd, uint16_t value, uint8_t *out)
{
    /* TODO: Assemble 6-byte frame into out array */
    (void)cmd; (void)value; (void)out;
}

int validate_command_frame(const uint8_t *in)
{
    /* TODO: Return 1 if frame has valid SOF, EOF, and checksum; 0 otherwise */
    (void)in;
    return 0;
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
