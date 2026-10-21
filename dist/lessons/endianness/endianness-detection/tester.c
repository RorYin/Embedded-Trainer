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
        printf("le: %d\n", system_is_little_endian());
    } else if (c == 2) {
        printf("swap: 0x%x\n", bswap16(0x1234));
    } else if (c == 3) {
        printf("swap: 0x%x\n", bswap16(0x00FF));
    } else if (c == 4) {
        printf("swap: 0x%x\n", bswap32(0x12345678));
    } else if (c == 5) {
        printf("swap: 0x%x\n", bswap32(0xAABBCCDD));
    }
    return 0;
}
