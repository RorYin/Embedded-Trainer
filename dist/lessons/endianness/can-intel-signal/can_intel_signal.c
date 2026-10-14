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

int main(void)
{
    /* Payload: byte0 = 0xAA, byte1 = 0x55 */
    /* 0xAA = 10101010b, 0x55 = 01010101b */
    uint8_t payload[2] = { 0xAA, 0x55 };

    /* Extract 8 bits starting at bit 0 -> byte 0 = 0xAA */
    if (can_extract_intel_signal16(payload, 0, 8) != 0xAA) { printf("Failed: bit 0 len 8\n"); return 1; }

    /* Extract 8 bits starting at bit 8 -> byte 1 = 0x55 */
    if (can_extract_intel_signal16(payload, 8, 8) != 0x55) { printf("Failed: bit 8 len 8\n"); return 2; }

    /* Extract 8 bits starting at bit 4 (crosses boundary: bits [7:4] of byte0 + bits [3:0] of byte1) */
    /* byte0[7:4] = 0xA (1010b), byte1[3:0] = 0x5 (0101b) -> 0x5A */
    if (can_extract_intel_signal16(payload, 4, 8) != 0x5A) { printf("Failed: cross boundary 0x5A\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
