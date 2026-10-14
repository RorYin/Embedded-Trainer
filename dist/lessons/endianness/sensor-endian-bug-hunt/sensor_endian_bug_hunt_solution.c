#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint16_t sensor_id;
    uint32_t pressure_pa;
    uint16_t status_flags;
} SensorData;

SensorData parse_sensor_packet_fixed(const uint8_t *buf)
{
    SensorData d;

    /* Sensor ID: Little Endian */
    d.sensor_id = (uint16_t)(((uint16_t)buf[1] << 8) | buf[0]);

    /* Pressure: Big Endian */
    d.pressure_pa = ((uint32_t)buf[2] << 24) |
                    ((uint32_t)buf[3] << 16) |
                    ((uint32_t)buf[4] <<  8) |
                    ((uint32_t)buf[5]);

    /* Status Flags: Big Endian */
    d.status_flags = (uint16_t)(((uint16_t)buf[6] << 8) | buf[7]);

    return d;
}

int main(void)
{
    uint8_t packet[8] = {
        0x34, 0x12,             /* Sensor ID: LE 0x1234 */
        0x00, 0x01, 0x8B, 0xCD, /* Pressure: BE 101325 */
        0x80, 0x01              /* Flags: BE 0x8001 */
    };

    SensorData s = parse_sensor_packet_fixed(packet);

    if (s.sensor_id != 0x1234) {
        printf("Failed: sensor_id, got 0x%x\n", s.sensor_id);
        return 1;
    }
    if (s.pressure_pa != 101325) {
        printf("Failed: pressure, got %d\n", (int)s.pressure_pa);
        return 2;
    }
    if (s.status_flags != 0x8001) {
        printf("Failed: status_flags, got 0x%x\n", s.status_flags);
        return 3;
    }

    printf("All tests passed!\n");
    return 0;
}
