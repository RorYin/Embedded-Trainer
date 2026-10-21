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
