#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint16_t transaction_id;
    uint16_t protocol_id;
    uint16_t length;
    uint8_t  unit_id;
} ModbusTCPHeader;

int parse_modbus_tcp_header(const uint8_t *frame, uint16_t frame_len, ModbusTCPHeader *out)
{
    if (frame == NULL || out == NULL || frame_len < 7) {
        return -1;
    }

    uint16_t proto = ((uint16_t)frame[2] << 8) | (uint16_t)frame[3];
    if (proto != 0) {
        return -1;
    }

    out->transaction_id = ((uint16_t)frame[0] << 8) | (uint16_t)frame[1];
    out->protocol_id    = proto;
    out->length         = ((uint16_t)frame[4] << 8) | (uint16_t)frame[5];
    out->unit_id        = frame[6];
    return 0;
}

void build_modbus_tcp_header(uint8_t *out, uint16_t transaction_id, uint8_t unit_id, uint8_t pdu_len)
{
    out[0] = (uint8_t)(transaction_id >> 8);
    out[1] = (uint8_t)(transaction_id & 0xFF);
    out[2] = 0x00;
    out[3] = 0x00;
    uint16_t total_len = 1 + pdu_len;
    out[4] = (uint8_t)(total_len >> 8);
    out[5] = (uint8_t)(total_len & 0xFF);
    out[6] = unit_id;
}
