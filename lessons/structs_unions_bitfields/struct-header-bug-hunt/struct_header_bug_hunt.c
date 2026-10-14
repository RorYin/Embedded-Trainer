#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Packet Header Serialization
 *
 * A protocol defines the following header format:
 *   Byte 0    : Packet Type
 *   Bytes 1..2: Payload length (Big Endian)
 *   Bytes 3..6: Sequence number (Big Endian)
 */

void build_header_fixed(uint8_t *out, uint8_t type, uint16_t payload_len, uint32_t seq)
{
    /* TODO: Fix bugs and serialize header properly in big-endian order */
    (void)out; (void)type; (void)payload_len; (void)seq;
}

int main(void)
{
    uint8_t hdr[7];
    build_header_fixed(hdr, 0x82, 0x0150, 0x00A1B2C3);

    if (hdr[0] != 0x82) { printf("Failed: type\n"); return 1; }

    /* Length 0x0150 in Big Endian */
    if (hdr[1] != 0x01 || hdr[2] != 0x50) {
        printf("Failed: length BE: %x %x\n", hdr[1], hdr[2]);
        return 2;
    }

    /* Seq 0x00A1B2C3 in Big Endian */
    if (hdr[3] != 0x00 || hdr[4] != 0xA1 || hdr[5] != 0xB2 || hdr[6] != 0xC3) {
        printf("Failed: seq BE: %x %x %x %x\n", hdr[3], hdr[4], hdr[5], hdr[6]);
        return 3;
    }

    printf("All tests passed!\n");
    return 0;
}
