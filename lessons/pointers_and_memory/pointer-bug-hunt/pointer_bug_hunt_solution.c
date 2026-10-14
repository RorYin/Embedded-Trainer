#include <stdio.h>
#include <stdint.h>

uint8_t find_max_fixed(const uint8_t *arr, uint8_t count)
{
    if (arr == NULL || count == 0) {
        return 0;
    }

    uint8_t max_val = arr[0];
    for (uint8_t i = 1; i < count; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

int main(void)
{
    uint8_t data[5];
    data[0] = 3; data[1] = 7; data[2] = 2; data[3] = 9; data[4] = 1;
    if (find_max_fixed(data, 5) != 9) { printf("Failed: max of data\n"); return 1; }

    uint8_t single[1];
    single[0] = 42;
    if (find_max_fixed(single, 1) != 42) { printf("Failed: single\n"); return 2; }

    if (find_max_fixed(NULL, 5) != 0) { printf("Failed: NULL check\n"); return 3; }

    if (find_max_fixed(data, 0) != 0) { printf("Failed: zero count\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
