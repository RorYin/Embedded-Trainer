#include <stdio.h>
#include <stdint.h>

uint8_t debounce_button(uint8_t raw_sample)
{
    static uint8_t debounced_state = 0;
    static uint8_t last_sample = 0;
    static uint8_t match_count = 0;

    if (raw_sample == last_sample) {
        if (match_count < 3) {
            match_count++;
        }
    } else {
        last_sample = raw_sample;
        match_count = 1;
    }

    if (match_count >= 3) {
        debounced_state = last_sample;
    }

    return debounced_state;
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
