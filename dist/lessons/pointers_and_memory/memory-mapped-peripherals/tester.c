#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef PERIPH_CR_EN
#define PERIPH_CR_EN   (1 << 0)
#define PERIPH_SR_BUSY (1 << 1)
#define PERIPH_SR_DONE (1 << 0)
#endif

void periph_init(FakePeripheral *p);
int periph_wait_ready(FakePeripheral *p, uint32_t timeout_loops);
void periph_write(FakePeripheral *p, uint32_t data);

static int read_case_num(void) {
    int ch;
    int num = 0;
    while ((ch = getchar()) != -1) {
        if (ch >= '0' && ch <= '9') {
            num = ch - '0';
            while ((ch = getchar()) != -1 && ch >= '0' && ch <= '9') {
                num = num * 10 + (ch - '0');
            }
            return num;
        }
    }
    return 1;
}

int main(void) {
    int test_case = read_case_num();
    FakePeripheral *dev = (FakePeripheral*)malloc(sizeof(FakePeripheral));
    dev->CR = 0;
    dev->SR = PERIPH_SR_BUSY;
    dev->DR = 0;

    if (test_case == 1) {
        periph_init(dev);
        printf("%d\n", (dev->CR & PERIPH_CR_EN) != 0 ? 1 : 0);
    } else if (test_case == 2) {
        printf("%d\n", periph_wait_ready(dev, 10));
    } else if (test_case == 3) {
        dev->SR &= ~PERIPH_SR_BUSY;
        printf("%d\n", periph_wait_ready(dev, 10));
    } else if (test_case == 4) {
        periph_write(dev, 0xCAFEBABE);
        printf("0x%x\n", (unsigned int)dev->DR);
    } else {
        printf("Unknown test case %d\n", test_case);
    }

    free(dev);
    return 0;
}
