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
    GPIO_TypeDef port[1];
    port->MODER = 0;
    port->BSRR = 0;
    port->IDR = 0;

    if (c == 1) {
        gpio_set_mode(port, 5, GPIO_MODE_OUTPUT);
        printf("moder: 0x%x\n", port->MODER);
    } else if (c == 2) {
        gpio_set_mode(port, 5, GPIO_MODE_OUTPUT);
        gpio_set_mode(port, 5, GPIO_MODE_ANALOG);
        printf("moder: 0x%x\n", port->MODER);
    } else if (c == 3) {
        gpio_write_pin_atomic(port, 3, 1);
        printf("bsrr: 0x%x\n", port->BSRR);
    } else if (c == 4) {
        gpio_write_pin_atomic(port, 3, 0);
        printf("bsrr: 0x%x\n", port->BSRR);
    } else if (c == 5) {
        port->IDR = 0x0004;
        printf("pin2: %d pin0: %d\n", gpio_read_input(port, 2), gpio_read_input(port, 0));
    }
    return 0;
}
