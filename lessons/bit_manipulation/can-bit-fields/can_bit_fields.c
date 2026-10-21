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
