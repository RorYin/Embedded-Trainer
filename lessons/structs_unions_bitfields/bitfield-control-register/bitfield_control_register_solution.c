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

int main(void)
{
    uint32_t val = build_control_reg(1, 0, 1, 1);
    if (val != 0x00000105) {
        printf("Failed: expected 0x00000105, got 0x%x\n", val);
        return 1;
    }

    if (get_control_speed(val) != 1) { printf("Failed: extract speed 1\n"); return 2; }
    if (get_control_speed(0x00000300) != 3) { printf("Failed: extract speed 3\n"); return 3; }

    printf("All tests passed!\n");
    return 0;
}
