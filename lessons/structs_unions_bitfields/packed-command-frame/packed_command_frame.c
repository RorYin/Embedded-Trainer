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
