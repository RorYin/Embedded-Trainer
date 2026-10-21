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
        printf("dac: %d\n", pwm_to_dac(0));
    } else if (c == 2) {
        printf("dac: %d\n", pwm_to_dac(50));
    } else if (c == 3) {
        printf("dac: %d\n", pwm_to_dac(100));
    } else if (c == 4) {
        printf("dac: %d\n", pwm_to_dac(120));
    } else if (c == 5) {
        printf("dac: %d\n", pwm_to_dac(48));
    } else if (c == 6) {
        printf("dac: %d\n", pwm_to_dac(24));
    }
    return 0;
}
