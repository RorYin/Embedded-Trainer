#include <stdio.h>
#include <stdint.h>

/*
 * TASK 1: Basic Register Operations
 * In embedded systems, GPIO pins are controlled by memory-mapped registers.
 */

/* Sets bit 'pin' in *reg without altering any other bits */
void gpio_set_pin(uint16_t *reg, uint8_t pin)
{
    /* TODO: Set bit 'pin' */
    (void)reg; (void)pin;
}

/* Clears bit 'pin' in *reg without altering any other bits */
void gpio_clear_pin(uint16_t *reg, uint8_t pin)
{
    /* TODO: Clear bit 'pin' */
    (void)reg; (void)pin;
}

/* Toggles bit 'pin' in *reg */
void gpio_toggle_pin(uint16_t *reg, uint8_t pin)
{
    /* TODO: Toggle bit 'pin' */
    (void)reg; (void)pin;
}

/* Returns 1 if bit 'pin' is set, 0 if cleared */
uint8_t gpio_read_pin(uint16_t reg, uint8_t pin)
{
    /* TODO: Read bit 'pin' */
    (void)reg; (void)pin;
    return 0;
}
