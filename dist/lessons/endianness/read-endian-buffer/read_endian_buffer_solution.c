#include <stdio.h>
#include <stdint.h>

uint16_t read_be16(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[0] << 8) | (uint16_t)buf[1]);
}

uint32_t read_be32(const uint8_t *buf)
{
    return ((uint32_t)buf[0] << 24) |
           ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] <<  8) |
           ((uint32_t)buf[3]);
}

uint16_t read_le16(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[1] << 8) | (uint16_t)buf[0]);
}

uint32_t read_le32(const uint8_t *buf)
{
    return ((uint32_t)buf[3] << 24) |
           ((uint32_t)buf[2] << 16) |
           ((uint32_t)buf[1] <<  8) |
           ((uint32_t)buf[0]);
}

int main(void)
{
    uint8_t buffer[4] = { 0x12, 0x34, 0x56, 0x78 };

    if (read_be16(buffer) != 0x1234) { printf("Failed: read_be16\n"); return 1; }
    if (read_be32(buffer) != 0x12345678) { printf("Failed: read_be32\n"); return 2; }

    if (read_le16(buffer) != 0x3412) { printf("Failed: read_le16\n"); return 3; }
    if (read_le32(buffer) != 0x78563412) { printf("Failed: read_le32\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
