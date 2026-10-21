#include <stdio.h>
#include <stdint.h>

int is_little_endian(void);
uint16_t swap16(uint16_t x);
uint32_t swap32(uint32_t x);

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
        printf("%d\n", is_little_endian());
    } else if (test_case == 2) {
        printf("0x%x\n", (unsigned int)swap16(0xAABB));
    } else if (test_case == 3) {
        printf("0x%x\n", (unsigned int)swap16(0x1234));
    } else if (test_case == 4) {
        printf("0x%x\n", (unsigned int)swap32(0xAABBCCDD));
    } else if (test_case == 5) {
        printf("0x%x\n", (unsigned int)swap32(0x12345678));
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
