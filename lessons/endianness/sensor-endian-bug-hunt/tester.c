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
    uint8_t packet[8];

    if (c == 1) {
        packet[0] = 0x34; packet[1] = 0x12;
        packet[2] = 0x00; packet[3] = 0x01; packet[4] = 0x8B; packet[5] = 0xCD;
        packet[6] = 0x80; packet[7] = 0x01;
        SensorData s = parse_sensor_packet_fixed(packet);
        uint16_t sid = s.sensor_id;
        uint32_t press = s.pressure_pa;
        uint16_t flg = s.status_flags;
        printf("id: 0x%x press: %d flags: 0x%x\n", sid, press, flg);
    } else if (c == 2) {
        int i;
        for (i = 0; i < 8; i++) packet[i] = 0;
        SensorData s = parse_sensor_packet_fixed(packet);
        uint16_t sid = s.sensor_id;
        uint32_t press = s.pressure_pa;
        uint16_t flg = s.status_flags;
        printf("id: 0x%x press: %d flags: 0x%x\n", sid, press, flg);
    } else if (c == 3) {
        packet[0] = 0xFF; packet[1] = 0xFF;
        packet[2] = 0x00; packet[3] = 0x0F; packet[4] = 0x42; packet[5] = 0x40;
        packet[6] = 0x7F; packet[7] = 0xFF;
        SensorData s = parse_sensor_packet_fixed(packet);
        uint16_t sid = s.sensor_id;
        uint32_t press = s.pressure_pa;
        uint16_t flg = s.status_flags;
        printf("id: 0x%x press: %d flags: 0x%x\n", sid, press, flg);
    } else if (c == 4) {
        packet[0] = 0x01; packet[1] = 0x80;
        packet[2] = 0x00; packet[3] = 0x00; packet[4] = 0x03; packet[5] = 0xE8;
        packet[6] = 0x00; packet[7] = 0x05;
        SensorData s = parse_sensor_packet_fixed(packet);
        uint16_t sid = s.sensor_id;
        uint32_t press = s.pressure_pa;
        uint16_t flg = s.status_flags;
        printf("id: 0x%x press: %d flags: 0x%x\n", sid, press, flg);
    }
    return 0;
}
