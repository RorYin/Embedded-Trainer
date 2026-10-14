#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Pointer Pitfalls in Array Max Search
 */

uint8_t find_max_fixed(const uint8_t *arr, uint8_t count)
{
    /* TODO: Fix all bugs. Return 0 if arr is NULL or count is 0 */
    (void)arr; (void)count;
    return 0;
}

int main(void)
{
    /* Test 1: Normal array */
    uint8_t data[5];
    data[0] = 3; data[1] = 7; data[2] = 2; data[3] = 9; data[4] = 1;
    if (find_max_fixed(data, 5) != 9) { printf("Failed: max of data\n"); return 1; }

    /* Test 2: Single element */
    uint8_t single[1];
    single[0] = 42;
    if (find_max_fixed(single, 1) != 42) { printf("Failed: single\n"); return 2; }

    /* Test 3: NULL pointer protection */
    if (find_max_fixed(NULL, 5) != 0) { printf("Failed: NULL check\n"); return 3; }

    /* Test 4: Zero length protection */
    if (find_max_fixed(data, 0) != 0) { printf("Failed: zero count\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
