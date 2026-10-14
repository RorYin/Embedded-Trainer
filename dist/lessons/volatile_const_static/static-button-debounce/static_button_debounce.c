#include <stdio.h>
#include <stdint.h>

/*
 * TASK 3: Static Variables for State Retention in Button Debouncing
 */

uint8_t debounce_button(uint8_t raw_sample)
{
    /* TODO: Use static variables to retain state and sample match count */
    (void)raw_sample;
    return 0;
}

int main(void)
{
    if (debounce_button(0) != 0) { printf("Failed initial sample\n"); return 1; }

    if (debounce_button(1) != 0) { printf("Failed bounce 1\n"); return 2; }
    if (debounce_button(0) != 0) { printf("Failed bounce 2\n"); return 3; }
    if (debounce_button(1) != 0) { printf("Failed bounce 3\n"); return 4; }

    if (debounce_button(1) != 0) { printf("Failed sample 2\n"); return 5; }
    if (debounce_button(1) != 1) { printf("Failed: should confirm press 1\n"); return 6; }

    if (debounce_button(1) != 1) { printf("Failed sustained 1\n"); return 7; }

    printf("All tests passed!\n");
    return 0;
}
