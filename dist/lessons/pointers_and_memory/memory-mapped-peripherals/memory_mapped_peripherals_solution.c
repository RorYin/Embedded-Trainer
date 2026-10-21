#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t RESERVED;
} FakePeripheral;

#define PERIPH_CR_EN   (1 << 0)
#define PERIPH_SR_BUSY (1 << 1)
#define PERIPH_SR_DONE (1 << 0)

void periph_init(FakePeripheral *p)
{
    p->CR |= PERIPH_CR_EN;
}

int periph_wait_ready(FakePeripheral *p, uint32_t timeout_loops)
{
    while ((p->SR & PERIPH_SR_BUSY) && timeout_loops > 0) {
        timeout_loops--;
    }
    return (p->SR & PERIPH_SR_BUSY) ? -1 : 0;
}

void periph_write(FakePeripheral *p, uint32_t data)
{
    p->DR = data;
}
