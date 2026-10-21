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

