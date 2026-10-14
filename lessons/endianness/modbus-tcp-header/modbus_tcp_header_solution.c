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

int main(void)
{
    uint8_t raw[7];
    raw[0] = 0x00; raw[1] = 0x01; raw[2] = 0x00; raw[3] = 0x00;
    raw[4] = 0x00; raw[5] = 0x06; raw[6] = 0x01;
    ModbusTCPHeader *hdr = (ModbusTCPHeader*)malloc(sizeof(ModbusTCPHeader));

    if (parse_modbus_tcp_header(raw, 7, hdr) != 0) { printf("Failed: parse\n"); free(hdr); return 1; }
    if (hdr->transaction_id != 1) { printf("Failed: TID\n"); free(hdr); return 2; }
    if (hdr->protocol_id != 0) { printf("Failed: Proto\n"); free(hdr); return 3; }
    if (hdr->length != 6) { printf("Failed: Len\n"); free(hdr); return 4; }
    if (hdr->unit_id != 1) { printf("Failed: Unit\n"); free(hdr); return 5; }
    free(hdr);

    uint8_t out[7];
    build_modbus_tcp_header(out, 0x0042, 0x05, 10);
    if (out[0] != 0x00 || out[1] != 0x42) { printf("Failed: build TID\n"); return 6; }
    if (out[2] != 0x00 || out[3] != 0x00) { printf("Failed: build Proto\n"); return 7; }
    if (out[4] != 0x00 || out[5] != 0x0B) { printf("Failed: build Len (1+10=11=0x0B)\n"); return 8; }
    if (out[6] != 0x05) { printf("Failed: build Unit\n"); return 9; }

    printf("All tests passed!\n");
    return 0;
}
