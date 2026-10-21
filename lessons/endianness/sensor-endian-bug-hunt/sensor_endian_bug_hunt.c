#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Multi-Byte Sensor Packet Endianness
 *
 * Packet Format (8 bytes):
 *   Bytes [0..1]: Sensor ID     (16-bit Little Endian)
 *   Bytes [2..5]: Pressure (Pa) (32-bit Big Endian)
 *   Bytes [6..7]: Status Flags  (16-bit Big Endian)
 *
 * Bugs to find and fix:
 *   Bug 1: Sensor ID decoded big-endian instead of little-endian!
 *   Bug 2: Pressure decoded little-endian instead of big-endian!
 *   Bug 3: Status flags shifted by wrong byte order.
 */

typedef struct {
    uint16_t sensor_id;
    uint32_t pressure_pa;
    uint16_t status_flags;
} SensorData;

SensorData parse_sensor_packet_fixed(const uint8_t *buf)
{
    SensorData d;
    /* TODO: Fix endianness bugs for each field */
    d.sensor_id = 0;
    d.pressure_pa = 0;
    d.status_flags = 0;
    (void)buf;
    return d;
}
