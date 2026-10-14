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

int main(void)
{
    uint8_t raw[4];
    raw[0] = 0xEF; raw[1] = 0xBE; raw[2] = 0xAD; raw[3] = 0xDE;

    uint32_t val = bytes_to_u32_pun(raw);
    if (val != 0xDEADBEEF) {
        printf("Failed: got 0x%x\n", val);
        return 1;
    }

    uint8_t buf[4];
    my_memcpy(buf, raw, 4);
    if (buf[0] != 0xEF || buf[3] != 0xDE) {
        printf("Failed: my_memcpy\n");
        return 2;
    }

    printf("All tests passed!\n");
    return 0;
}
