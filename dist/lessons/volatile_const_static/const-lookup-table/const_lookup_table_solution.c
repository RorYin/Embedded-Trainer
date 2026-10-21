#include <stdio.h>
#include <stdint.h>

uint8_t pwm_to_dac(uint8_t pwm_percent)
{
    uint8_t table[11];
    table[0] = 0;   table[1] = 26;  table[2] = 51;  table[3] = 77;
    table[4] = 102; table[5] = 128; table[6] = 153; table[7] = 179;
    table[8] = 204; table[9] = 230; table[10] = 255;

    if (pwm_percent > 100) {
        pwm_percent = 100;
    }
    uint8_t index = (uint8_t)((pwm_percent + 5) / 10);
    return table[index];
}
