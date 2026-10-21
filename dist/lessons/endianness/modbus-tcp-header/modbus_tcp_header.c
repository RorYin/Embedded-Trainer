#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * TASK 4: Modbus TCP MBAP Header Parsing & Construction
 *
 * MBAP Header Layout (7 bytes, Big Endian):
 *   Bytes [0..1]: Transaction Identifier (uint16_t)
 *   Bytes [2..3]: Protocol Identifier    (uint16_t, must be 0 for Modbus)
 *   Bytes [4..5]: Length                 (uint16_t, number of following bytes)
 *   Byte 6      : Unit Identifier        (uint8_t)
 */

typedef struct {
    uint16_t transaction_id;
    uint16_t protocol_id;
    uint16_t length;
    uint8_t  unit_id;
} ModbusTCPHeader;

/*
 * Parse MBAP header from raw frame.
 * Validate that frame_len >= 7 and protocol_id == 0.
 * Return 0 on success, -1 on invalid frame.
 */
int parse_modbus_tcp_header(const uint8_t *frame, uint16_t frame_len, ModbusTCPHeader *out)
{
    /* TODO: Parse MBAP header */
    (void)frame; (void)frame_len; (void)out;
    return -1;
}

/*
 * Build MBAP header into out buffer (7 bytes) for a request.
 * Sets protocol_id = 0, length = 1 (unit_id) + pdu_len.
 */
void build_modbus_tcp_header(uint8_t *out, uint16_t transaction_id, uint8_t unit_id, uint8_t pdu_len)
{
    /* TODO: Serialize MBAP header */
    (void)out; (void)transaction_id; (void)unit_id; (void)pdu_len;
}
