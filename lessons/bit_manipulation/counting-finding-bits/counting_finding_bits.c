#include <stdio.h>
#include <stdint.h>

/*
 * TASK 4: Counting & Finding Bits
 */

/* Returns number of set bits (1s) in x (Brian Kernighan's algorithm) */
uint8_t count_set_bits(uint32_t x)
{
    /* TODO: Count set bits */
    (void)x;
    return 0;
}

/* Returns the index (0..31) of the most significant set bit, or -1 if x == 0 */
int8_t find_highest_set_bit(uint32_t x)
{
    /* TODO: Find index of highest set bit */
    (void)x;
    return -1;
}

/* Reverses all 32 bits (bit 0 becomes bit 31, etc.) */
uint32_t reverse_bits(uint32_t x)
{
    /* TODO: Reverse 32 bits */
    (void)x;
    return 0;
}

int main(void)
{
    if (count_set_bits(0xB5) != 5) { printf("Failed: count_set_bits(0xB5)\n"); return 1; }
    if (count_set_bits(0) != 0) { printf("Failed: count_set_bits(0)\n"); return 2; }
    if (count_set_bits(0xFFFFFFFF) != 32) { printf("Failed: count_set_bits(all)\n"); return 3; }

    if (find_highest_set_bit(0x0A) != 3) { printf("Failed: find_highest(10)\n"); return 4; }
    if (find_highest_set_bit(0) != -1) { printf("Failed: find_highest(0)\n"); return 5; }
    if (find_highest_set_bit(1) != 0) { printf("Failed: find_highest(1)\n"); return 6; }
    if (find_highest_set_bit(0x80000000) != 31) { printf("Failed: find_highest(msb)\n"); return 7; }

    if (reverse_bits(0x80000000) != 0x00000001) { printf("Failed: reverse msb\n"); return 8; }
    if (reverse_bits(0x00000001) != 0x80000000) { printf("Failed: reverse lsb\n"); return 9; }

    printf("All tests passed!\n");
    return 0;
}
