#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

void usart_enable(USART_TypeDef *u)
{
    u->CR1 |= USART_CR1_UE;
}

void usart_configure_8n1(USART_TypeDef *u)
{
    u->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS);
}

void usart_configure_8e1(USART_TypeDef *u)
{
    u->CR1 = (u->CR1 & ~(USART_CR1_M | USART_CR1_PS)) | USART_CR1_PCE;
}

void usart_configure_9o1(USART_TypeDef *u)
{
    u->CR1 |= (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS);
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
