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
        printf("size: %d\n", get_s1_size());
    } else if (c == 2) {
        printf("size: %d\n", get_s2_size());
    } else if (c == 3) {
        printf("size: %d\n", get_s3_size());
    } else if (c == 4) {
        printf("size: %d\n", get_s4_size());
    }
    return 0;
}
