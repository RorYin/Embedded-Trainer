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

int main(void)
{
    /* Frame: TID=0x0001, Proto=0x0000, Len=0x0006, Unit=0x01 */
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

    /* Build check */
    uint8_t out[7];
    build_modbus_tcp_header(out, 0x0042, 0x05, 10);
    if (out[0] != 0x00 || out[1] != 0x42) { printf("Failed: build TID\n"); return 6; }
    if (out[2] != 0x00 || out[3] != 0x00) { printf("Failed: build Proto\n"); return 7; }
    if (out[4] != 0x00 || out[5] != 0x0B) { printf("Failed: build Len (1+10=11=0x0B)\n"); return 8; }
    if (out[6] != 0x05) { printf("Failed: build Unit\n"); return 9; }

    printf("All tests passed!\n");
    return 0;
}
