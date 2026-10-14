#include <stdio.h>
#include <stdint.h>

/*
 * TASK 5: Const Correctness for Buffer Summation & ROM Table Lookup
 */

/* Sum all bytes in a read-only buffer */
uint32_t sum_bytes(const uint8_t *buf, uint16_t len)
{
    /* TODO: Iterate and sum buffer bytes without modifying buf */
    (void)buf; (void)len;
    return 0;
}

/* Compute simple 3-bit index CRC lookup */
uint8_t lookup_crc(uint8_t index)
{
    /* TODO: Return lookup for index & 7 */
    (void)index;
    return 0;
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
