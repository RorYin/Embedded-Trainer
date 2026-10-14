#include <stdio.h>
#include <stdint.h>

void gpio_set_pin(uint16_t *reg, uint8_t pin)
{
    *reg |= (1 << pin);
}

void gpio_clear_pin(uint16_t *reg, uint8_t pin)
{
    *reg &= ~(1 << pin);
}

void gpio_toggle_pin(uint16_t *reg, uint8_t pin)
{
    *reg ^= (1 << pin);
}

uint8_t gpio_read_pin(uint16_t reg, uint8_t pin)
{
    return (reg >> pin) & 1;
}

int main(void)
{
    uint16_t reg[1];
    reg[0] = 0x0000;

    gpio_set_pin(reg, 3);
    if (reg[0] != 0x0008) { printf("Failed: set pin 3\n"); return 1; }

    gpio_set_pin(reg, 7);
    if (reg[0] != 0x0088) { printf("Failed: set pin 7\n"); return 2; }

    gpio_clear_pin(reg, 3);
    if (reg[0] != 0x0080) { printf("Failed: clear pin 3\n"); return 3; }

    gpio_toggle_pin(reg, 0);
    if (reg[0] != 0x0081) { printf("Failed: toggle pin 0\n"); return 4; }

    if (gpio_read_pin(reg[0], 0) != 1) { printf("Failed: read pin 0\n"); return 5; }
    if (gpio_read_pin(reg[0], 1) != 0) { printf("Failed: read pin 1\n"); return 6; }

    printf("All tests passed!\n");
    return 0;
}
