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
