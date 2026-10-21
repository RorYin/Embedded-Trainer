#include <stdio.h>
#include <stdint.h>

static int read_case_num(void) {
    int ch;
    int num = 0;
    while ((ch = getchar()) != -1) {
        if (ch >= '0' && ch <= '9') {
            num = ch - '0';
            while ((ch = getchar()) != -1 && ch >= '0' && ch <= '9') {
                num = num * 10 + (ch - '0');
            }
            return num;
        }
    }
    return 1;
}

int main(void) {
    int c = read_case_num();
    ModbusTCPHeader hdr[1];
    uint8_t raw[7];
    raw[0] = 0x00; raw[1] = 0x01; raw[2] = 0x00; raw[3] = 0x00;
    raw[4] = 0x00; raw[5] = 0x06; raw[6] = 0x01;

    if (c == 1) {
        int r = parse_modbus_tcp_header(raw, 7, hdr);
        printf("res: %d tid: %d proto: %d len: %d unit: %d\n",
               r, hdr->transaction_id, hdr->protocol_id, hdr->length, hdr->unit_id);
    } else if (c == 2) {
        raw[3] = 0x01;
        int r = parse_modbus_tcp_header(raw, 7, hdr);
        printf("res: %d\n", r);
    } else if (c == 3) {
        int r = parse_modbus_tcp_header(raw, 6, hdr);
        printf("res: %d\n", r);
    } else if (c == 4) {
        int r = parse_modbus_tcp_header(NULL, 7, hdr);
        printf("res: %d\n", r);
    } else if (c == 5) {
        uint8_t out[7];
        build_modbus_tcp_header(out, 0x0042, 0x05, 10);
        printf("out: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
               out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
    }
    return 0;
}
