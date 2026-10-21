#include <stdio.h>
#include <stdint.h>

uint16_t can_extract_intel_signal16(const uint8_t *payload, uint8_t start_bit, uint8_t bit_len)
{
    uint8_t byte_idx = start_bit / 8;
    uint8_t bit_offset = start_bit % 8;

    /* Assemble consecutive bytes into a 32-bit window */
    uint32_t window = (uint32_t)payload[byte_idx] |
                      ((uint32_t)payload[byte_idx + 1] << 8);

    uint16_t shifted = (uint16_t)(window >> bit_offset);
    uint16_t mask = (bit_len >= 16) ? 0xFFFF : (uint16_t)((1 << bit_len) - 1);
    return shifted & mask;
}
