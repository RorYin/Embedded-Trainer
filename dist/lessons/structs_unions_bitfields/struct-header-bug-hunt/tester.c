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
    uint8_t hdr[7];

    if (c == 1) {
        build_header_fixed(hdr, 0x82, 0x0150, 0x00A1B2C3);
        printf("hdr: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
               hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6]);
    } else if (c == 2) {
        build_header_fixed(hdr, 0x01, 0x0004, 0x00000001);
        printf("hdr: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
               hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6]);
    } else if (c == 3) {
        build_header_fixed(hdr, 0xFF, 0xFFFF, 0xFFFFFFFF);
        printf("hdr: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
               hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6]);
    } else if (c == 4) {
        build_header_fixed(hdr, 0x00, 0x0000, 0x12345678);
        printf("hdr: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
               hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6]);
    }
    return 0;
}
