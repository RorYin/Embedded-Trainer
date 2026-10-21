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
