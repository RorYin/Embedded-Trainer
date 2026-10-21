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
        g_hw_timer_counter = 500;
        printf("count: %d\n", read_timer_count());
    } else if (c == 2) {
        g_hw_timer_counter = 500;
        uint32_t start = read_timer_count();
        g_hw_timer_counter = 540;
        printf("expired: %d\n", has_timer_expired(start, 50));
    } else if (c == 3) {
        g_hw_timer_counter = 500;
        uint32_t start = read_timer_count();
        g_hw_timer_counter = 550;
        printf("expired: %d\n", has_timer_expired(start, 50));
    } else if (c == 4) {
        g_hw_timer_counter = 500;
        uint32_t start = read_timer_count();
        g_hw_timer_counter = 600;
        printf("expired: %d\n", has_timer_expired(start, 50));
    } else if (c == 5) {
        uint32_t roll_start = 0xFFFFFFF0;
        g_hw_timer_counter = 0x00000010;
        printf("expired: %d\n", has_timer_expired(roll_start, 30));
    }
    return 0;
}
