#include <stdio.h>
#include <stdint.h>

/*
 * TASK 4: Read-Only Hardware Register with const volatile
 */

static volatile uint32_t g_hw_timer_counter = 0;

uint32_t read_timer_count(void)
{
    /* TODO: Return current value from g_hw_timer_counter */
    return 0;
}

uint8_t has_timer_expired(uint32_t start_tick, uint32_t duration_ticks)
{
    /* TODO: Check if duration has elapsed handling timer wraparound */
    (void)start_tick; (void)duration_ticks;
    return 0;
}
