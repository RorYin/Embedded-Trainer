#include <stdio.h>
#include <stdint.h>

/*
 * TASK 5: CAN Intel Signal Extraction Across Byte Boundaries
 *
 * In automotive CAN databases (DBC), signals are packed at arbitrary bit
 * offsets and lengths (little-endian / Intel format).
 */

/*
 * Extracts an Intel signal of up to 16 bits starting at start_bit.
 * Start_bit is the absolute bit index in the payload:
 *   byte_index = start_bit / 8
 *   bit_offset = start_bit % 8
 */
uint16_t can_extract_intel_signal16(const uint8_t *payload, uint8_t start_bit, uint8_t bit_len)
{
    /* TODO: Extract signal across byte boundaries */
    (void)payload; (void)start_bit; (void)bit_len;
    return 0;
}
