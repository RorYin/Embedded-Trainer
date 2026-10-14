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

int main(void)
{
    /* Packet: SensorID=0x1234 (LE), Pressure=101325=0x00018BCD (BE), Flags=0x8001 (BE) */
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
