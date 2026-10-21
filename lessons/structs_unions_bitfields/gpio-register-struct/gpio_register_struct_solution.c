#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

void gpio_set_mode(GPIO_TypeDef *gpio, uint8_t pin, uint8_t mode)
{
    uint32_t shift = (uint32_t)pin * 2;
    gpio->MODER = (gpio->MODER & ~(0x3 << shift)) | ((uint32_t)(mode & 0x3) << shift);
}

void gpio_write_pin_atomic(GPIO_TypeDef *gpio, uint8_t pin, uint8_t value)
{
    if (value) {
        gpio->BSRR = (1 << pin);
    } else {
        gpio->BSRR = (1 << (pin + 16));
    }
}

uint8_t gpio_read_input(GPIO_TypeDef *gpio, uint8_t pin)
{
    return (gpio->IDR >> pin) & 1;
}
