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
