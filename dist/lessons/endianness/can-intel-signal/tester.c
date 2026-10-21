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
    uint8_t payload[4];
    payload[0] = 0xAA; payload[1] = 0x55; payload[2] = 0xF0; payload[3] = 0x0F;

    if (c == 1) {
        printf("sig: 0x%x\n", can_extract_intel_signal16(payload, 0, 8));
    } else if (c == 2) {
        printf("sig: 0x%x\n", can_extract_intel_signal16(payload, 8, 8));
    } else if (c == 3) {
        printf("sig: 0x%x\n", can_extract_intel_signal16(payload, 4, 8));
    } else if (c == 4) {
        printf("sig: 0x%x\n", can_extract_intel_signal16(payload, 0, 16));
    } else if (c == 5) {
        printf("sig: 0x%x\n", can_extract_intel_signal16(payload, 12, 12));
    }
    return 0;
}
