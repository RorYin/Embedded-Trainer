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
    uint8_t buf[4];

    if (c == 1) {
        write_be16(buf, 0xABCD);
        printf("be16: 0x%x 0x%x\n", buf[0], buf[1]);
    } else if (c == 2) {
        write_be32(buf, 0x11223344);
        printf("be32: 0x%x 0x%x 0x%x 0x%x\n", buf[0], buf[1], buf[2], buf[3]);
    } else if (c == 3) {
        write_le16(buf, 0xABCD);
        printf("le16: 0x%x 0x%x\n", buf[0], buf[1]);
    } else if (c == 4) {
        write_be16(buf, 0x0001);
        printf("be16: 0x%x 0x%x ", buf[0], buf[1]);
        write_le16(buf, 0x0001);
        printf("le16: 0x%x 0x%x\n", buf[0], buf[1]);
    } else if (c == 5) {
        write_be32(buf, 0xAABBCCDD);
        printf("be32: 0x%x 0x%x 0x%x 0x%x\n", buf[0], buf[1], buf[2], buf[3]);
    }
    return 0;
}
