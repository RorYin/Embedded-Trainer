#include <stdio.h>
#include <stdint.h>

/*
 * TASK 5: Module-Private Encapsulation using File-Scope Static
 */

static uint8_t  s_duty_pct = 0;
static uint32_t s_period_ticks = 1000;

void pwm_set_duty(uint8_t duty_pct, uint32_t period_ticks)
{
    /* TODO: Set s_duty_pct and s_period_ticks */
    (void)duty_pct; (void)period_ticks;
}

uint8_t pwm_get_output(uint32_t current_tick)
{
    /* TODO: Compute PWM output level */
    (void)current_tick;
    return 0;
}
