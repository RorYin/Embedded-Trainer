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
