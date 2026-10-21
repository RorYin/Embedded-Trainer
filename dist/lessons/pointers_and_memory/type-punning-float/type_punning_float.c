#include <stdio.h>
#include <stdint.h>

/*
 * TASK 3: Safe Type Punning & Byte Copying
 */

void my_memcpy(uint8_t *dst, const uint8_t *src, uint16_t n)
{
    /* TODO: Copy n bytes from src to dst */
    (void)dst; (void)src; (void)n;
}

uint32_t bytes_to_u32_pun(const uint8_t *bytes)
{
    /* TODO: Safely unpack 4 bytes into a uint32_t word (Little Endian)
     * using my_memcpy to avoid unaligned pointer dereference faults.
     * Hint: uint32_t val[1]; my_memcpy((uint8_t*)val, bytes, 4); return val[0];
     */
    (void)bytes;
    return 0;
}
