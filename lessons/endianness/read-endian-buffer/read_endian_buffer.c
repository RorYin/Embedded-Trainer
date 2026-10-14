#include <stdio.h>
#include <stdint.h>

/*
 * TASK 2: Reading Multi-Byte Values from Big-Endian & Little-Endian Buffers
 */

/* Read 16-bit Big-Endian value from buffer */
uint16_t read_be16(const uint8_t *buf)
{
    /* TODO: Read BE 16-bit */
    (void)buf;
    return 0;
}

/* Read 32-bit Big-Endian value from buffer */
uint32_t read_be32(const uint8_t *buf)
{
    /* TODO: Read BE 32-bit */
    (void)buf;
    return 0;
}

/* Read 16-bit Little-Endian value from buffer */
uint16_t read_le16(const uint8_t *buf)
{
    /* TODO: Read LE 16-bit */
    (void)buf;
    return 0;
}

/* Read 32-bit Little-Endian value from buffer */
uint32_t read_le32(const uint8_t *buf)
{
    /* TODO: Read LE 32-bit */
    (void)buf;
    return 0;
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
