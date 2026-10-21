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
