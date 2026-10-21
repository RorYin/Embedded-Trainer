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
    static UartFrame f;
    /* TODO: Parse raw byte buffer using pointer arithmetic (*(raw + offset)) */
    f.sof = 0;
    f.cmd = 0;
    f.length = 0;
    f.payload = 0;
    f.checksum = 0;
    return f;
}
