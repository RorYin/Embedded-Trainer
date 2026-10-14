#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * TASK 2: Memory-Mapped Peripheral Access & Polling
 */

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t RESERVED;
} FakePeripheral;

#define PERIPH_CR_EN   (1 << 0)
#define PERIPH_SR_BUSY (1 << 1)
#define PERIPH_SR_DONE (1 << 0)

/* Enable peripheral by setting the EN bit in CR */
void periph_init(FakePeripheral *p)
{
    /* TODO: Set CR_EN */
    (void)p;
}

/*
 * Wait for peripheral to not be busy (SR_BUSY == 0).
 * Loop up to timeout_loops times.
 * Return 0 on success, -1 on timeout.
 */
int periph_wait_ready(FakePeripheral *p, uint32_t timeout_loops)
{
    /* TODO: Poll SR_BUSY */
    (void)p; (void)timeout_loops;
    return -1;
}

/* Write data to peripheral data register (DR) */
void periph_write(FakePeripheral *p, uint32_t data)
{
    /* TODO: Write to DR */
    (void)p; (void)data;
}

int main(void)
{
    FakePeripheral *dev = (FakePeripheral*)malloc(sizeof(FakePeripheral));
    dev->CR = 0;
    dev->SR = PERIPH_SR_BUSY;
    dev->DR = 0;

    periph_init(dev);
    if ((dev->CR & PERIPH_CR_EN) == 0) { printf("Failed: periph_init\n"); return 1; }

    /* Busy timeout check */
    if (periph_wait_ready(dev, 10) != -1) { printf("Failed: timeout\n"); return 2; }

    /* Clear busy */
    dev->SR &= ~PERIPH_SR_BUSY;
    if (periph_wait_ready(dev, 10) != 0) { printf("Failed: ready check\n"); return 3; }

    periph_write(dev, 0xCAFEBABE);
    if (dev->DR != 0xCAFEBABE) { printf("Failed: DR write\n"); return 4; }

    free(dev);
    printf("All tests passed!\n");
    return 0;
}
