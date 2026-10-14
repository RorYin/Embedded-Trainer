#include <stdio.h>
#include <stdint.h>

/*
 * TASK 1: Parse UART Frame using Pointer Arithmetic Only
 *
 * Frame Format:
 *   Byte 0    : SOF (Start of Frame delimiter, e.g. 0xAA)
 *   Byte 1    : Command ID
 *   Bytes 2..3: Payload Length (16-bit, Little Endian)
 *   Bytes 4..7: Payload Data   (32-bit, Little Endian)
 *   Byte 8    : Checksum (XOR of bytes 0..7)
 */

typedef struct {
    uint8_t  sof;
    uint8_t  cmd;
    uint16_t length;
    uint32_t payload;
    uint8_t  checksum;
} UartFrame;

UartFrame parse_frame(const uint8_t *raw)
{
    UartFrame f;
    /* TODO: Parse raw byte buffer using pointer arithmetic (*(raw + offset)) */
    f.sof = 0;
    f.cmd = 0;
    f.length = 0;
    f.payload = 0;
    f.checksum = 0;
    return f;
}

int main(void)
{
    uint8_t frame[9];
    frame[0] = 0xAA;
    frame[1] = 0x05;
    frame[2] = 0x04;
    frame[3] = 0x00;
    frame[4] = 0x78;
    frame[5] = 0x56;
    frame[6] = 0x34;
    frame[7] = 0x12;
    frame[8] = 0x55;

    UartFrame f = parse_frame(frame);

    if (f.sof != 0xAA) { printf("Failed: SOF\n"); return 1; }
    if (f.cmd != 0x05) { printf("Failed: CMD\n"); return 2; }
    if (f.length != 4) { printf("Failed: Length\n"); return 3; }
    if (f.payload != 0x12345678) { printf("Failed: Payload\n"); return 4; }
    if (f.checksum != 0x55) { printf("Failed: Checksum\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
