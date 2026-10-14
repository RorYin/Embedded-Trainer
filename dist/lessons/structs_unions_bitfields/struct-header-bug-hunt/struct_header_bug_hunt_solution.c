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

int main(void)
{
    uint8_t hdr[7];
    build_header_fixed(hdr, 0x82, 0x0150, 0x00A1B2C3);

    if (hdr[0] != 0x82) { printf("Failed: type\n"); return 1; }

    if (hdr[1] != 0x01 || hdr[2] != 0x50) {
        printf("Failed: length BE: %x %x\n", hdr[1], hdr[2]);
        return 2;
    }

    if (hdr[3] != 0x00 || hdr[4] != 0xA1 || hdr[5] != 0xB2 || hdr[6] != 0xC3) {
        printf("Failed: seq BE: %x %x %x %x\n", hdr[3], hdr[4], hdr[5], hdr[6]);
        return 3;
    }

    printf("All tests passed!\n");
    return 0;
}
