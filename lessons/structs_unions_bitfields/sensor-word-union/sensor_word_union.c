#include <stdio.h>
#include <stdint.h>

/*
 * TASK 4: Decoding Sensor Frame using Union
 *
 * A 4-byte sensor word contains:
 *   Bytes [0..1]: Temperature in tenths of a degree C (signed 16-bit, Big Endian)
 *   Bytes [2..3]: Humidity in tenths of a percent (unsigned 16-bit, Big Endian)
 */

void parse_sensor_word(const uint8_t *data, int16_t *temperature, uint16_t *humidity)
{
    /* TODO: Parse big-endian temperature and humidity */
    (void)data; (void)temperature; (void)humidity;
}
