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
        printf("state: %d\n", led_blink_step_fixed(0, 500));
    } else if (c == 2) {
        uint8_t s1 = led_blink_step_fixed(200, 500);
        uint8_t s2 = led_blink_step_fixed(500, 500);
        uint8_t s3 = led_blink_step_fixed(700, 500);
        uint8_t s4 = led_blink_step_fixed(1000, 500);
        printf("states: %d %d %d %d\n", s1, s2, s3, s4);
    } else if (c == 3) {
        uint8_t s1 = led_blink_step_fixed(100, 100);
        uint8_t s2 = led_blink_step_fixed(250, 100);
        printf("states: %d %d\n", s1, s2);
    } else if (c == 4) {
        uint8_t s1 = led_blink_step_fixed(50, 500);
        uint8_t s2 = led_blink_step_fixed(100, 500);
        uint8_t s3 = led_blink_step_fixed(499, 500);
        printf("states: %d %d %d\n", s1, s2, s3);
    }
    return 0;
}
