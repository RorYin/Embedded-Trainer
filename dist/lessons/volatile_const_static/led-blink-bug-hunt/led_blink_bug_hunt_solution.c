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

int main(void)
{
    uint8_t s = led_blink_step_fixed(0, 500);
    if (s != 0) { printf("Failed: initial tick 0\n"); return 1; }

    s = led_blink_step_fixed(200, 500);
    if (s != 0) { printf("Failed: tick 200\n"); return 2; }

    s = led_blink_step_fixed(500, 500);
    if (s != 1) { printf("Failed: toggle to 1 at tick 500\n"); return 3; }

    s = led_blink_step_fixed(700, 500);
    if (s != 1) { printf("Failed: tick 700\n"); return 4; }

    s = led_blink_step_fixed(1000, 500);
    if (s != 0) { printf("Failed: toggle to 0 at tick 1000\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
