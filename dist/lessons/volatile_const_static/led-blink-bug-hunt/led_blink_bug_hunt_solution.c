#include <stdio.h>
#include <stdint.h>

uint8_t led_blink_step_fixed(uint32_t current_tick, uint32_t interval_ticks)
{
    static uint32_t last_toggle = 0;
    static uint8_t  led_state = 0;

    if ((current_tick - last_toggle) >= interval_ticks) {
        led_state = !led_state;
        last_toggle = current_tick;
    }

    return led_state;
}
