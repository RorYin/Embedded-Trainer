#include <stdio.h>
#include <stdint.h>

/*
 * TASK 2: Const Lookup Tables in ROM/Flash
 */

uint8_t pwm_to_dac(uint8_t pwm_percent)
{
    /* TODO: Clamp to 100, round to nearest 10%, return lookup value */
    (void)pwm_percent;
    return 0;
}

int main(void)
{
    if (pwm_to_dac(0) != 0) { printf("Failed: 0 percent\n"); return 1; }
    if (pwm_to_dac(50) != 128) { printf("Failed: 50 percent\n"); return 2; }
    if (pwm_to_dac(100) != 255) { printf("Failed: 100 percent\n"); return 3; }
    if (pwm_to_dac(120) != 255) { printf("Failed: clamp 120 percent\n"); return 4; }
    if (pwm_to_dac(48) != 128) { printf("Failed: rounding 48 -> 50\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
