#include <stdio.h>
#include <stdint.h>

static uint8_t  s_duty_pct = 0;
static uint32_t s_period_ticks = 1000;

void pwm_set_duty(uint8_t duty_pct, uint32_t period_ticks)
{
    if (duty_pct > 100) duty_pct = 100;
    s_duty_pct = duty_pct;
    s_period_ticks = period_ticks ? period_ticks : 1;
}

uint8_t pwm_get_output(uint32_t current_tick)
{
    if (s_duty_pct == 0) return 0;
    if (s_duty_pct == 100) return 1;

    uint32_t phase = current_tick % s_period_ticks;
    uint32_t on_time = (s_period_ticks * s_duty_pct) / 100;

    return (phase < on_time) ? 1 : 0;
}

int main(void)
{
    pwm_set_duty(50, 1000);

    if (pwm_get_output(0) != 1)   { printf("Failed: tick 0\n"); return 1; }
    if (pwm_get_output(499) != 1) { printf("Failed: tick 499\n"); return 2; }
    if (pwm_get_output(500) != 0) { printf("Failed: tick 500\n"); return 3; }
    if (pwm_get_output(999) != 0) { printf("Failed: tick 999\n"); return 4; }

    if (pwm_get_output(1000) != 1) { printf("Failed: tick 1000\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
