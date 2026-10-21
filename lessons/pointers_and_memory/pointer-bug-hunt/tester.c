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
        uint8_t data[5];
        data[0] = 3; data[1] = 7; data[2] = 2; data[3] = 9; data[4] = 1;
        printf("max: %d\n", find_max_fixed(data, 5));
    } else if (c == 2) {
        uint8_t single[1];
        single[0] = 42;
        printf("max: %d\n", find_max_fixed(single, 1));
    } else if (c == 3) {
        printf("max: %d\n", find_max_fixed(NULL, 5));
    } else if (c == 4) {
        uint8_t data[5];
        data[0] = 3;
        printf("max: %d\n", find_max_fixed(data, 0));
    } else if (c == 5) {
        uint8_t data[5];
        data[0] = 100; data[1] = 200; data[2] = 50; data[3] = 255; data[4] = 128;
        printf("max: %d\n", find_max_fixed(data, 5));
    }
    return 0;
}
