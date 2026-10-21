#include <stdio.h>
#include <stdint.h>

void parse_sensor_word(const uint8_t *data, int16_t *temperature, uint16_t *humidity)
{
    int16_t raw_temp = (int16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
    uint16_t raw_hum = ((uint16_t)data[2] << 8) | (uint16_t)data[3];

    *temperature = raw_temp;
    *humidity = raw_hum;
}
