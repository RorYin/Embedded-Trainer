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
        printf("state: %d\n", debounce_button(0));
    } else if (c == 2) {
        printf("state: %d", debounce_button(1));
        printf(" %d", debounce_button(0));
        printf(" %d\n", debounce_button(1));
    } else if (c == 3) {
        printf("state: %d", debounce_button(1));
        printf(" %d", debounce_button(1));
        printf(" %d\n", debounce_button(1));
    } else if (c == 4) {
        debounce_button(1);
        debounce_button(1);
        debounce_button(1);
        printf("state: %d", debounce_button(0));
        printf(" %d", debounce_button(1));
        printf(" %d", debounce_button(0));
        printf(" %d", debounce_button(0));
        printf(" %d\n", debounce_button(0));
    }
    return 0;
}
