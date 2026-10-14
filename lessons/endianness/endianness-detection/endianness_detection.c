#include <stdio.h>
#include <stdint.h>

/*
 * TASK 1: Runtime Endianness Detection and Byte Swapping
 */

/* Returns 1 if CPU is little-endian, 0 if big-endian */
int system_is_little_endian(void)
{
    /* TODO: Detect endianness */
    return 0;
}

/* Swap bytes in 16-bit word (0x1234 -> 0x3412) */
uint16_t bswap16(uint16_t x)
{
    /* TODO: Byte swap 16-bit word */
    (void)x;
    return 0;
}

/* Swap bytes in 32-bit dword (0x12345678 -> 0x78563412) */
uint32_t bswap32(uint32_t x)
{
    /* TODO: Byte swap 32-bit dword */
    (void)x;
    return 0;
}

int main(void)
{
    if (system_is_little_endian() != 1) { printf("Failed: endianness check\n"); return 1; }
    if (bswap16(0x1234) != 0x3412) { printf("Failed: bswap16\n"); return 2; }
    if (bswap32(0x12345678) != 0x78563412) { printf("Failed: bswap32\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
