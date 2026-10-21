#include <stdio.h>
#include <stdint.h>

uint16_t set_field_fixed(uint16_t reg, uint8_t val);

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

    if (test_case == 1) {
        printf("0x%x\n", (unsigned int)set_field_fixed(0xF00F, 0x5));
    } else if (test_case == 2) {
        printf("0x%x\n", (unsigned int)set_field_fixed(0x12F4, 0x0));
    } else if (test_case == 3) {
        printf("0x%x\n", (unsigned int)set_field_fixed(0x0000, 0xFF));
    } else if (test_case == 4) {
        printf("0x%x\n", (unsigned int)set_field_fixed(0xAAAA, 0x5));
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
