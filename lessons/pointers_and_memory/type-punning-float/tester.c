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
    if (c == 1) {
        uint8_t raw[4];
        raw[0] = 0xEF; raw[1] = 0xBE; raw[2] = 0xAD; raw[3] = 0xDE;
        uint32_t val = bytes_to_u32_pun(raw);
        printf("u32: 0x%x\n", val);
    } else if (c == 2) {
        uint8_t raw[4];
        raw[0] = 0x01; raw[1] = 0x02; raw[2] = 0x03; raw[3] = 0x04;
        uint32_t val = bytes_to_u32_pun(raw);
        printf("u32: 0x%x\n", val);
    } else if (c == 3) {
        uint8_t raw[4];
        raw[0] = 0x00; raw[1] = 0x00; raw[2] = 0x00; raw[3] = 0x00;
        uint32_t val = bytes_to_u32_pun(raw);
        printf("u32: 0x%x\n", val);
    } else if (c == 4) {
        uint8_t src[8];
        uint8_t dst[8];
        int i;
        for (i = 0; i < 8; i++) {
            src[i] = (uint8_t)(0x10 + i);
            dst[i] = 0;
        }
        my_memcpy(dst, src, 8);
        printf("dst:");
        for (i = 0; i < 8; i++) {
            printf(" 0x%x", dst[i]);
        }
        printf("\n");
    } else if (c == 5) {
        uint8_t src[4];
        uint8_t dst[4];
        int i;
        for (i = 0; i < 4; i++) {
            src[i] = (uint8_t)(0xAA);
            dst[i] = (uint8_t)(0x55);
        }
        my_memcpy(dst, src, 0);
        printf("dst[0]: 0x%x\n", dst[0]);
    }
    return 0;
}
