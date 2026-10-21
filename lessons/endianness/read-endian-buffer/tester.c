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
    uint8_t buffer[4];
    buffer[0] = 0x12; buffer[1] = 0x34; buffer[2] = 0x56; buffer[3] = 0x78;

    if (c == 1) {
        printf("be16: 0x%x\n", read_be16(buffer));
    } else if (c == 2) {
        printf("be32: 0x%x\n", read_be32(buffer));
    } else if (c == 3) {
        printf("le16: 0x%x\n", read_le16(buffer));
    } else if (c == 4) {
        printf("le32: 0x%x\n", read_le32(buffer));
    } else if (c == 5) {
        uint8_t buffer2[4];
        buffer2[0] = 0xAA; buffer2[1] = 0xBB; buffer2[2] = 0xCC; buffer2[3] = 0xDD;
        printf("be: 0x%x le: 0x%x\n", read_be32(buffer2), read_le32(buffer2));
    }
    return 0;
}
