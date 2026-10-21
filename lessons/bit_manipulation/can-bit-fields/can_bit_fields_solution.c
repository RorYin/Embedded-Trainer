#include <stdio.h>
#include <stdint.h>

uint8_t can_get_tx_count(uint32_t r)
{
    return (r >> 0) & 0xFF;
}

uint8_t can_get_rx_count(uint32_t r)
{
    return (r >> 8) & 0xFF;
}

uint8_t can_get_error_counter(uint32_t r)
{
    return (r >> 16) & 0xFF;
}

uint32_t can_set_tx_count(uint32_t reg, uint8_t count)
{
    return (reg & ~0x000000FF) | (count & 0xFF);
}
