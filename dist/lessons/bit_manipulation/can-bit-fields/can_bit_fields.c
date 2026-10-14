#include <stdio.h>
#include <stdint.h>

/*
 * TASK 2: Bit Field Extraction & Insertion
 * CAN controller status register layout:
 *   Bits [7:0]   : TX error counter (tx_count)
 *   Bits [15:8]  : RX error counter (rx_count)
 *   Bits [23:16] : Error warning counter (error_counter)
 *   Bits [31:24] : Reserved (do not touch)
 */

uint8_t can_get_tx_count(uint32_t status_reg)
{
    /* TODO: Return bits [7:0] */
    (void)status_reg;
    return 0;
}

uint8_t can_get_rx_count(uint32_t status_reg)
{
    /* TODO: Return bits [15:8] */
    (void)status_reg;
    return 0;
}

uint8_t can_get_error_counter(uint32_t status_reg)
{
    /* TODO: Return bits [23:16] */
    (void)status_reg;
    return 0;
}

uint32_t can_set_tx_count(uint32_t status_reg, uint8_t count)
{
    /* TODO: Replace bits [7:0] with count, leaving all other bits intact */
    (void)status_reg; (void)count;
    return 0;
}

int main(void)
{
    uint32_t can_status = 0x00AB1234;

    if (can_get_tx_count(can_status) != 0x34) { printf("Failed: tx_count\n"); return 1; }
    if (can_get_rx_count(can_status) != 0x12) { printf("Failed: rx_count\n"); return 2; }
    if (can_get_error_counter(can_status) != 0xAB) { printf("Failed: error_counter\n"); return 3; }

    uint32_t updated = can_set_tx_count(can_status, 0xFF);
    if ((updated & 0xFF) != 0xFF) { printf("Failed: set tx_count value\n"); return 4; }
    if ((updated >> 8) != (can_status >> 8)) { printf("Failed: preserve other bits\n"); return 5; }

    printf("All tests passed!\n");
    return 0;
}
