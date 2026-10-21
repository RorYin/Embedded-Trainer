#include <stdio.h>
#include <stdint.h>

#define CR_PE_SHIFT    0
#define CR_TXIE_SHIFT  1
#define CR_RXIE_SHIFT  2
#define CR_SPEED_SHIFT 8

uint32_t build_control_reg(uint8_t pe, uint8_t txie, uint8_t rxie, uint8_t speed)
{
    uint32_t reg = 0;
    if (pe)   reg |= (1 << CR_PE_SHIFT);
    if (txie) reg |= (1 << CR_TXIE_SHIFT);
    if (rxie) reg |= (1 << CR_RXIE_SHIFT);
    reg |= ((uint32_t)(speed & 0x03) << CR_SPEED_SHIFT);
    return reg;
}

uint8_t get_control_speed(uint32_t reg)
{
    return (uint8_t)((reg >> CR_SPEED_SHIFT) & 0x03);
}
