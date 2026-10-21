#include <stdio.h>
#include <stdint.h>

static volatile uint32_t g_hw_timer_counter = 0;

uint32_t read_timer_count(void)
{
    return g_hw_timer_counter;
}

uint8_t has_timer_expired(uint32_t start_tick, uint32_t duration_ticks)
{
    uint32_t current = read_timer_count();
    return (current - start_tick) >= duration_ticks;
}
