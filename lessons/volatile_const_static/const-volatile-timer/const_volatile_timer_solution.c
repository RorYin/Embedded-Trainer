#include <stdio.h>
#include <stdint.h>

static volatile uint32_t g_hw_timer_counter[1];

uint32_t read_timer_count(void)
{
    return g_hw_timer_counter[0];
}

uint8_t has_timer_expired(uint32_t start_tick, uint32_t duration_ticks)
{
    uint32_t current = read_timer_count();
    return (current - start_tick) >= duration_ticks;
}

int main(void)
{
    g_hw_timer_counter[0] = 500;
    if (read_timer_count() != 500) { printf("Failed: read count\n"); return 1; }

    uint32_t start = read_timer_count();

    g_hw_timer_counter[0] = 540;
    if (has_timer_expired(start, 50) != 0) { printf("Failed: not expired yet\n"); return 2; }

    g_hw_timer_counter[0] = 550;
    if (has_timer_expired(start, 50) != 1) { printf("Failed: exactly expired\n"); return 3; }

    uint32_t roll_start = 0xFFFFFFF0;
    g_hw_timer_counter[0] = 0x00000010;
    if (has_timer_expired(roll_start, 30) != 1) { printf("Failed: rollover check\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
