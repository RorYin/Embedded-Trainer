#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * TASK 5: USART Packed Register Configuration
 * CR1 bit assignments:
 *   Bit 13: UE  (USART Enable)
 *   Bit 12: M   (Word Length: 0 = 8 data bits, 1 = 9 data bits)
 *   Bit 10: PCE (Parity Control Enable: 0 = disabled, 1 = enabled)
 *   Bit 9:  PS  (Parity Selection: 0 = Even, 1 = Odd)
 */

#define USART_CR1_UE  (1 << 13)
#define USART_CR1_M   (1 << 12)
#define USART_CR1_PCE (1 << 10)
#define USART_CR1_PS  (1 << 9)

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t SR;
    volatile uint32_t DR;
} USART_TypeDef;

/* Set the UE (USART Enable) bit in CR1 */
void usart_enable(USART_TypeDef *u)
{
    /* TODO: Enable USART */
    (void)u;
}

/* 8 data bits, no parity: M=0, PCE=0, PS=0 */
void usart_configure_8n1(USART_TypeDef *u)
{
    /* TODO: Configure 8N1 */
    (void)u;
}

/* 8 data bits, even parity: M=0, PCE=1, PS=0 */
void usart_configure_8e1(USART_TypeDef *u)
{
    /* TODO: Configure 8E1 */
    (void)u;
}

/* 9 data bits, odd parity: M=1, PCE=1, PS=1 */
void usart_configure_9o1(USART_TypeDef *u)
{
    /* TODO: Configure 9O1 */
    (void)u;
}

int main(void)
{
    USART_TypeDef *usart = (USART_TypeDef*)malloc(sizeof(USART_TypeDef));
    usart->CR1 = 0;

    usart_enable(usart);
    if ((usart->CR1 & USART_CR1_UE) == 0) { printf("Failed: usart_enable\n"); return 1; }

    usart->CR1 = 0xFFFF;
    usart_configure_8n1(usart);
    if (usart->CR1 & (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS)) { printf("Failed: 8N1\n"); return 2; }

    usart_configure_8e1(usart);
    if ((usart->CR1 & USART_CR1_PCE) == 0) { printf("Failed: 8E1 PCE\n"); return 3; }
    if (usart->CR1 & (USART_CR1_M | USART_CR1_PS)) { printf("Failed: 8E1 clear M/PS\n"); return 4; }

    usart_configure_9o1(usart);
    if ((usart->CR1 & (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS)) != (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS)) {
        printf("Failed: 9O1\n"); return 5;
    }

    free(usart);
    printf("All tests passed!\n");
    return 0;
}
