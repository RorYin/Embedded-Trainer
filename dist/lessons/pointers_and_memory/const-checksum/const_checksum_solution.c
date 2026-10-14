#include <stdio.h>
#include <stdint.h>

uint32_t sum_bytes(const uint8_t *buf, uint16_t len)
{
    uint32_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return sum;
}

uint8_t lookup_crc(uint8_t index)
{
    uint8_t crc_table[8];
    crc_table[0] = 0x00;
    crc_table[1] = 0x07;
    crc_table[2] = 0x0E;
    crc_table[3] = 0x09;
    crc_table[4] = 0x1C;
    crc_table[5] = 0x1B;
    crc_table[6] = 0x12;
    crc_table[7] = 0x15;
    return crc_table[index & 7];
}

int main(void)
{
    uint8_t data[4];
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40;

    if (sum_bytes(data, 4) != 100) { printf("Failed: sum_bytes\n"); return 1; }

    if (lookup_crc(0) != 0x00) { printf("Failed: crc 0\n"); return 2; }
    if (lookup_crc(1) != 0x07) { printf("Failed: crc 1\n"); return 3; }
    if (lookup_crc(7) != 0x15) { printf("Failed: crc 7\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
