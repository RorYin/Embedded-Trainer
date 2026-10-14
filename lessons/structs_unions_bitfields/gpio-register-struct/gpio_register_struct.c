#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * TASK 2: STM32-Style GPIO Register Map Struct
 */

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_TypeDef;

#define GPIO_MODE_INPUT  0x0
#define GPIO_MODE_OUTPUT 0x1
#define GPIO_MODE_AF     0x2
#define GPIO_MODE_ANALOG 0x3

/* Set 2-bit mode for given pin in MODER register */
void gpio_set_mode(GPIO_TypeDef *gpio, uint8_t pin, uint8_t mode)
{
    /* TODO: Clear 2-bit field at (pin * 2), then write mode */
    (void)gpio; (void)pin; (void)mode;
}

/*
 * Atomic pin write using BSRR register:
 *   Bits [15:0]  : Set pin (1 = set pin high)
 *   Bits [31:16] : Reset pin (1 = reset pin low)
 */
void gpio_write_pin_atomic(GPIO_TypeDef *gpio, uint8_t pin, uint8_t value)
{
    /* TODO: Write to BSRR atomically */
    (void)gpio; (void)pin; (void)value;
}

/* Read input state of given pin from IDR */
uint8_t gpio_read_input(GPIO_TypeDef *gpio, uint8_t pin)
{
    /* TODO: Read bit from IDR */
    (void)gpio; (void)pin;
    return 0;
}

int main(void)
{
    GPIO_TypeDef *port = (GPIO_TypeDef*)malloc(sizeof(GPIO_TypeDef));
    port->MODER = 0;
    port->BSRR = 0;
    port->IDR = 0;

    gpio_set_mode(port, 5, GPIO_MODE_OUTPUT);
    if ((port->MODER & (0x3 << 10)) != (0x1 << 10)) { printf("Failed: set mode 5\n"); return 1; }

    gpio_set_mode(port, 5, GPIO_MODE_ANALOG);
    if ((port->MODER & (0x3 << 10)) != (0x3 << 10)) { printf("Failed: update mode 5\n"); return 2; }

    /* Atomic set */
    gpio_write_pin_atomic(port, 3, 1);
    if (port->BSRR != (1 << 3)) { printf("Failed: atomic set 3\n"); return 3; }

    /* Atomic reset */
    gpio_write_pin_atomic(port, 3, 0);
    if (port->BSRR != (1 << (3 + 16))) { printf("Failed: atomic reset 3\n"); return 4; }

    port->IDR = 0x0004;
    if (gpio_read_input(port, 2) != 1) { printf("Failed: read input pin 2\n"); return 5; }
    if (gpio_read_input(port, 0) != 0) { printf("Failed: read input pin 0\n"); return 6; }

    free(port);
    printf("All tests passed!\n");
    return 0;
}
