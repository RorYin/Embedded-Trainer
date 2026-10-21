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
    uint8_t received[1];
    received[0] = 0;

    if (c == 1) {
        g_rx_ready = 0;
        int r = process_rx_data(received);
        printf("ret: %d ready: %d\n", r, g_rx_ready);
    } else if (c == 2) {
        g_rx_ready = 0;
        simulated_uart_isr(0x42);
        int r = process_rx_data(received);
        printf("ret: %d byte: 0x%x ready: %d\n", r, received[0], g_rx_ready);
    } else if (c == 3) {
        g_rx_ready = 0;
        simulated_uart_isr(0x42);
        process_rx_data(received);
        int r2 = process_rx_data(received);
        printf("ret: %d ready: %d\n", r2, g_rx_ready);
    } else if (c == 4) {
        g_rx_ready = 0;
        simulated_uart_isr(0xA5);
        int r = process_rx_data(received);
        printf("ret: %d byte: 0x%x ready: %d\n", r, received[0], g_rx_ready);
    } else if (c == 5) {
        g_rx_ready = 0;
        simulated_uart_isr(0x10);
        simulated_uart_isr(0x20);
        int r = process_rx_data(received);
        printf("ret: %d byte: 0x%x ready: %d\n", r, received[0], g_rx_ready);
    }
    return 0;
}
