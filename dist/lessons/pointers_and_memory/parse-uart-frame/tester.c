#include <stdio.h>
#include <stdint.h>

UartFrame parse_frame(const uint8_t *raw);

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
    int test_case = read_case_num();
    uint8_t frame[9];
    frame[0] = 0xAA;
    frame[1] = 0x05;
    frame[2] = 0x04;
    frame[3] = 0x00;
    frame[4] = 0x78;
    frame[5] = 0x56;
    frame[6] = 0x34;
    frame[7] = 0x12;
    frame[8] = 0x55;

    UartFrame f = parse_frame(frame);
    uint32_t sof = f.sof;
    uint32_t cmd = f.cmd;
    uint32_t len = f.length;
    uint32_t payload = f.payload;
    uint32_t checksum = f.checksum;

    if (test_case == 1) {
        printf("0x%x\n", sof);
    } else if (test_case == 2) {
        printf("0x%x\n", cmd);
    } else if (test_case == 3) {
        printf("%d\n", len);
    } else if (test_case == 4) {
        printf("0x%x\n", payload);
    } else if (test_case == 5) {
        printf("0x%x\n", checksum);
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
