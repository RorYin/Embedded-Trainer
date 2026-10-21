#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Non-Blocking LED Blinker
 */

uint8_t led_blink_step_fixed(uint32_t current_tick, uint32_t interval_ticks)
{
    /* TODO: Fix bugs using static variables to track last_toggle and led_state */
    (void)current_tick; (void)interval_ticks;
    return 0;
}
