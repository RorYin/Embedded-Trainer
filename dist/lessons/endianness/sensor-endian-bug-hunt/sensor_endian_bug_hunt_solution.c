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
