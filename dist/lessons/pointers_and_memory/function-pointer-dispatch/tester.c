#include <stdio.h>
#include <stdlib.h>
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
    init_dispatch_table();
    g_btn_count = 0;
    g_tmr_count = 0;
    g_uart_count = 0;

    if (c == 1) {
        int r = dispatch_event(0x01, 5);
        printf("res: %d, btn: %d, tmr: %d, uart: %d\n", r, g_btn_count, g_tmr_count, g_uart_count);
    } else if (c == 2) {
        int r = dispatch_event(0x02, 10);
        printf("res: %d, btn: %d, tmr: %d, uart: %d\n", r, g_btn_count, g_tmr_count, g_uart_count);
    } else if (c == 3) {
        int r = dispatch_event(0x03, 1);
        printf("res: %d, btn: %d, tmr: %d, uart: %d\n", r, g_btn_count, g_tmr_count, g_uart_count);
    } else if (c == 4) {
        int r = dispatch_event(0x99, 100);
        printf("res: %d, btn: %d, tmr: %d, uart: %d\n", r, g_btn_count, g_tmr_count, g_uart_count);
    } else if (c == 5) {
        dispatch_event(0x01, 2);
        dispatch_event(0x02, 4);
        dispatch_event(0x03, 6);
        dispatch_event(0x01, 8);
        printf("btn: %d, tmr: %d, uart: %d\n", g_btn_count, g_tmr_count, g_uart_count);
    }

    return 0;
}
