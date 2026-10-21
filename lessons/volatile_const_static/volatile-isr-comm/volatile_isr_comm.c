#include <stdio.h>
#include <stdint.h>

/*
 * TASK 1: Volatile Flag Synchronization between ISR and Main
 */

static volatile uint8_t g_rx_byte = 0;
static volatile uint8_t g_rx_ready = 0;

void simulated_uart_isr(uint8_t byte)
{
    g_rx_byte = byte;
    g_rx_ready = 1;
}

int process_rx_data(uint8_t *out_byte)
{
    /* TODO: Check g_rx_ready, copy to out_byte, clear flag, return status */
    (void)out_byte;
    return 0;
}
