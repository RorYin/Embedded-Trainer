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
        uint32_t val = build_control_reg(1, 0, 1, 1);
        printf("reg: 0x%x, speed: %d\n", val, get_control_speed(val));
    } else if (c == 2) {
        uint32_t val = build_control_reg(0, 0, 0, 0);
        printf("reg: 0x%x, speed: %d\n", val, get_control_speed(val));
    } else if (c == 3) {
        uint32_t val = build_control_reg(1, 1, 1, 3);
        printf("reg: 0x%x, speed: %d\n", val, get_control_speed(val));
    } else if (c == 4) {
        uint32_t val = build_control_reg(0, 1, 0, 2);
        printf("reg: 0x%x, speed: %d\n", val, get_control_speed(val));
    } else if (c == 5) {
        printf("speed: %d\n", get_control_speed(0x00000200));
    }
    return 0;
}
