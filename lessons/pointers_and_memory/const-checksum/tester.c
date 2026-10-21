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
        uint8_t data[4];
        data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40;
        printf("sum: %d\n", sum_bytes(data, 4));
    } else if (c == 2) {
        uint8_t data[4];
        data[0] = 255; data[1] = 255; data[2] = 255; data[3] = 255;
        printf("sum: %d\n", sum_bytes(data, 4));
    } else if (c == 3) {
        uint8_t data[1];
        data[0] = 0;
        printf("sum: %d\n", sum_bytes(data, 0));
    } else if (c == 4) {
        printf("crc: 0x%x 0x%x 0x%x\n", lookup_crc(0), lookup_crc(1), lookup_crc(7));
    } else if (c == 5) {
        printf("crc: 0x%x 0x%x\n", lookup_crc(9), lookup_crc(15));
    }
    return 0;
}
