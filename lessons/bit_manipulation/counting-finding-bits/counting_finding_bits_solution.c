#include <stdio.h>
#include <stdint.h>

uint8_t count_set_bits(uint32_t x)
{
    uint8_t count = 0;
    while (x) {
        x &= (x - 1);
        count++;
    }
    return count;
}

int8_t find_highest_set_bit(uint32_t x)
{
    if (x == 0) return -1;
    for (int i = 31; i >= 0; i--) {
        if ((x >> i) & 1) return (int8_t)i;
    }
    return -1;
}

uint32_t reverse_bits(uint32_t x)
{
    uint32_t res = 0;
    for (int i = 0; i < 32; i++) {
        res = (res << 1) | (x & 1);
        x >>= 1;
    }
    return res;
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
