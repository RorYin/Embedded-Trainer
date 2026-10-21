#include <stdio.h>
#include <stdint.h>

static int read_case_num(void) {
    int ch;
    int num = 0;
    while ((ch = getchar()) != -1) {
        if (ch >= '0' && ch <= '9') {
            num = ch - '0';
            while ((ch = getchar()) != -1 && ch >= '0' && ch <= '9') {
                num = num * 10 + (ch - '0');
            }
            return num;
        }
    }
    return 1;
}

int main(void) {
    int c = read_case_num();
    int16_t t[1];
    uint16_t h[1];
    t[0] = 0;
    h[0] = 0;

    if (c == 1) {
        uint8_t packet[4];
        packet[0] = 0x00; packet[1] = 0xFE; packet[2] = 0x02; packet[3] = 0x5D;
        parse_sensor_word(packet, t, h);
        printf("temp: %d hum: %d\n", t[0], (uint32_t)h[0] & 0xFFFF);
    } else if (c == 2) {
        uint8_t packet[4];
        packet[0] = 0xFF; packet[1] = 0x9C; packet[2] = 0x01; packet[3] = 0xF4;
        parse_sensor_word(packet, t, h);
        printf("temp: %d hum: %d\n", t[0], (uint32_t)h[0] & 0xFFFF);
    } else if (c == 3) {
        uint8_t packet[4];
        packet[0] = 0x00; packet[1] = 0x00; packet[2] = 0x00; packet[3] = 0x00;
        parse_sensor_word(packet, t, h);
        printf("temp: %d hum: %d\n", t[0], (uint32_t)h[0] & 0xFFFF);
    } else if (c == 4) {
        uint8_t packet[4];
        packet[0] = 0x7F; packet[1] = 0xFF; packet[2] = 0xFF; packet[3] = 0xFF;
        parse_sensor_word(packet, t, h);
        printf("temp: %d hum: %d\n", t[0], (uint32_t)h[0] & 0xFFFF);
    } else if (c == 5) {
        uint8_t packet[4];
        packet[0] = 0x80; packet[1] = 0x00; packet[2] = 0x03; packet[3] = 0xE8;
        parse_sensor_word(packet, t, h);
        printf("temp: %d hum: %d\n", t[0], (uint32_t)h[0] & 0xFFFF);
    }
    return 0;
}
