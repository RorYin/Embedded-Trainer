#include <stdio.h>
#include <stdint.h>

/*
 * TASK 6: BUG HUNT — Packet Header Serialization
 *
 * A protocol defines the following header format:
 *   Byte 0    : Packet Type
 *   Bytes 1..2: Payload length (Big Endian)
 *   Bytes 3..6: Sequence number (Big Endian)
 */

void build_header_fixed(uint8_t *out, uint8_t type, uint16_t payload_len, uint32_t seq)
{
    /* TODO: Fix bugs and serialize header properly in big-endian order */
    (void)out; (void)type; (void)payload_len; (void)seq;
}
