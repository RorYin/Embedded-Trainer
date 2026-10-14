#include <stdio.h>
#include <stdint.h>

/*
 * TASK 3: Endianness Detection & Byte Swap
 */

/* Return 1 if running on a little-endian system, 0 if big-endian */
int is_little_endian(void)
{
    /* TODO: Determine endianness at runtime */
    return 0;
}

/* Swap the 2 bytes of a 16-bit integer (e.g. 0xAABB -> 0xBBAA) */
uint16_t swap16(uint16_t x)
{
    /* TODO: Byte swap 16-bit word */
    (void)x;
    return 0;
}

/* Swap the 4 bytes of a 32-bit integer (e.g. 0xAABBCCDD -> 0xDDCCBBAA) */
uint32_t swap32(uint32_t x)
{
    /* TODO: Byte swap 32-bit dword */
    (void)x;
    return 0;
}

int main(void)
{
    if (is_little_endian() != 1) { printf("Failed: is_little_endian\n"); return 1; }
    if (swap16(0xAABB) != 0xBBAA) { printf("Failed: swap16\n"); return 2; }
    if (swap32(0xAABBCCDD) != 0xDDCCBBAA) { printf("Failed: swap32\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
