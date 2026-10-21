#include <stdio.h>
#include <stdint.h>

void gpio_set_pin(uint16_t *reg, uint8_t pin);
void gpio_clear_pin(uint16_t *reg, uint8_t pin);
void gpio_toggle_pin(uint16_t *reg, uint8_t pin);
uint8_t gpio_read_pin(uint16_t reg, uint8_t pin);

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
    int test_case = read_case_num();
    uint16_t reg[1];
    reg[0] = 0;

    if (test_case == 1) {
        reg[0] = 0x0000;
        gpio_set_pin(reg, 3);
        printf("0x%x\n", (unsigned int)reg[0]);
    } else if (test_case == 2) {
        reg[0] = 0x0008;
        gpio_set_pin(reg, 7);
        printf("0x%x\n", (unsigned int)reg[0]);
    } else if (test_case == 3) {
        reg[0] = 0x0088;
        gpio_clear_pin(reg, 3);
        printf("0x%x\n", (unsigned int)reg[0]);
    } else if (test_case == 4) {
        reg[0] = 0x0080;
        gpio_toggle_pin(reg, 0);
        printf("0x%x\n", (unsigned int)reg[0]);
    } else if (test_case == 5) {
        reg[0] = 0x0081;
        printf("%d\n", (int)gpio_read_pin(reg[0], 0));
    } else if (test_case == 6) {
        reg[0] = 0x0081;
        printf("%d\n", (int)gpio_read_pin(reg[0], 1));
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
