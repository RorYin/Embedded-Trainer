#include <stdio.h>
#include <stdint.h>

/*
 * TASK 3: Serializing Multi-Byte Integers to Big/Little-Endian Buffers
 */

/* Write 16-bit value in Big-Endian order into buf[0..1] */
void write_be16(uint8_t *buf, uint16_t val)
{
    /* TODO: Write BE 16-bit */
    (void)buf; (void)val;
}

/* Write 32-bit value in Big-Endian order into buf[0..3] */
void write_be32(uint8_t *buf, uint32_t val)
{
    /* TODO: Write BE 32-bit */
    (void)buf; (void)val;
}

/* Write 16-bit value in Little-Endian order into buf[0..1] */
void write_le16(uint8_t *buf, uint16_t val)
{
    /* TODO: Write LE 16-bit */
    (void)buf; (void)val;
}

int main(void)
{
    uint8_t buf[4];

    write_be16(buf, 0xABCD);
    if (buf[0] != 0xAB || buf[1] != 0xCD) { printf("Failed: write_be16\n"); return 1; }

    write_be32(buf, 0x11223344);
    if (buf[0] != 0x11 || buf[1] != 0x22 || buf[2] != 0x33 || buf[3] != 0x44) {
        printf("Failed: write_be32\n"); return 2;
    }

    write_le16(buf, 0xABCD);
    if (buf[0] != 0xCD || buf[1] != 0xAB) { printf("Failed: write_le16\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
