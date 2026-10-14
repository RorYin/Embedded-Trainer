#include <stdio.h>
#include <stdint.h>

void parse_sensor_word(const uint8_t *data, int16_t *temperature, uint16_t *humidity)
{
    int16_t raw_temp = (int16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
    uint16_t raw_hum = ((uint16_t)data[2] << 8) | (uint16_t)data[3];

    *temperature = raw_temp;
    *humidity = raw_hum;
}

int main(void)
{
    uint8_t packet1[4];
    packet1[0] = 0x00; packet1[1] = 0xFE; packet1[2] = 0x02; packet1[3] = 0x5D;
    int16_t t1[1];
    uint16_t h1[1];
    parse_sensor_word(packet1, t1, h1);

    if (t1[0] != 254) { printf("Failed: temp 254\n"); return 1; }
    if (h1[0] != 605) { printf("Failed: humidity 605\n"); return 2; }

    uint8_t packet2[4];
    packet2[0] = 0xFF; packet2[1] = 0x9C; packet2[2] = 0x01; packet2[3] = 0xF4;
    int16_t t2[1];
    uint16_t h2[1];
    parse_sensor_word(packet2, t2, h2);

    if (t2[0] != -100) { printf("Failed: negative temp -100\n"); return 3; }
    if (h2[0] != 500) { printf("Failed: humidity 500\n"); return 4; }

    printf("All tests passed!\n");
    return 0;
}
