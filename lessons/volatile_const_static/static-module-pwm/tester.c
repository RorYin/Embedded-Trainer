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
        pwm_set_duty(50, 1000);
        printf("out: %d %d %d %d %d\n",
               pwm_get_output(0), pwm_get_output(499),
               pwm_get_output(500), pwm_get_output(999),
               pwm_get_output(1000));
    } else if (c == 2) {
        pwm_set_duty(0, 1000);
        printf("out: %d %d %d\n",
               pwm_get_output(0), pwm_get_output(500), pwm_get_output(999));
    } else if (c == 3) {
        pwm_set_duty(100, 1000);
        printf("out: %d %d %d\n",
               pwm_get_output(0), pwm_get_output(500), pwm_get_output(999));
    } else if (c == 4) {
        pwm_set_duty(25, 200);
        printf("out: %d %d %d %d\n",
               pwm_get_output(0), pwm_get_output(49),
               pwm_get_output(50), pwm_get_output(199));
    } else if (c == 5) {
        pwm_set_duty(150, 1000);
        printf("out: %d %d\n", pwm_get_output(0), pwm_get_output(500));
    }
    return 0;
}
