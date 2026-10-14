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

int main(void)
{
    FakePeripheral *dev = (FakePeripheral*)malloc(sizeof(FakePeripheral));
    dev->CR = 0;
    dev->SR = PERIPH_SR_BUSY;
    dev->DR = 0;

    periph_init(dev);
    if ((dev->CR & PERIPH_CR_EN) == 0) { printf("Failed: periph_init\n"); return 1; }

    if (periph_wait_ready(dev, 10) != -1) { printf("Failed: timeout\n"); return 2; }

    dev->SR &= ~PERIPH_SR_BUSY;
    if (periph_wait_ready(dev, 10) != 0) { printf("Failed: ready check\n"); return 3; }

    periph_write(dev, 0xCAFEBABE);
    if (dev->DR != 0xCAFEBABE) { printf("Failed: DR write\n"); return 4; }

    free(dev);
    printf("All tests passed!\n");
    return 0;
}
