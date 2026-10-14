#include <stdio.h>
#include <stdint.h>

static volatile uint8_t g_rx_byte = 0;
static volatile uint8_t g_rx_ready = 0;

void simulated_uart_isr(uint8_t byte)
{
    g_rx_byte = byte;
    g_rx_ready = 1;
}

int process_rx_data(uint8_t *out_byte)
{
    if (g_rx_ready) {
        *out_byte = g_rx_byte;
        g_rx_ready = 0;
        return 1;
    }
    return 0;
}

int main(void)
{
    uint8_t received[1];

    g_rx_ready = 0;
    if (process_rx_data(received) != 0) { printf("Failed: initial ready\n"); return 1; }

    simulated_uart_isr(0x42);
    if (process_rx_data(received) != 1) { printf("Failed: process ready\n"); return 2; }
    if (received[0] != 0x42) { printf("Failed: byte match\n"); return 3; }
    if (g_rx_ready != 0) { printf("Failed: flag clear\n"); return 4; }

    if (process_rx_data(received) != 0) { printf("Failed: second read\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
