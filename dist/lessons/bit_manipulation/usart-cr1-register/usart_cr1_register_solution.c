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
