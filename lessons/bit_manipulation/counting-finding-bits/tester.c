#include <stdio.h>
#include <stdint.h>

uint8_t count_set_bits(uint32_t x);
int8_t find_highest_set_bit(uint32_t x);
uint32_t reverse_bits(uint32_t x);

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
        printf("%d\n", (int)count_set_bits(0xB5));
    } else if (test_case == 2) {
        printf("%d\n", (int)count_set_bits(0));
    } else if (test_case == 3) {
        printf("%d\n", (int)count_set_bits(0xFFFFFFFF));
    } else if (test_case == 4) {
        printf("%d\n", (int)find_highest_set_bit(0x0A));
    } else if (test_case == 5) {
        printf("%d\n", (int)find_highest_set_bit(0));
    } else if (test_case == 6) {
        printf("%d\n", (int)find_highest_set_bit(0x80000000));
    } else if (test_case == 7) {
        printf("0x%x\n", (unsigned int)reverse_bits(0x80000000));
    } else if (test_case == 8) {
        printf("0x%x\n", (unsigned int)reverse_bits(0x00000001));
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
