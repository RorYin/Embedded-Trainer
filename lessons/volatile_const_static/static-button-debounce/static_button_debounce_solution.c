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
