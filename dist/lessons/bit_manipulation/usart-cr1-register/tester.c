#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void usart_enable(USART_TypeDef *u);
void usart_configure_8n1(USART_TypeDef *u);
void usart_configure_8e1(USART_TypeDef *u);
void usart_configure_9o1(USART_TypeDef *u);

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
    USART_TypeDef *usart = (USART_TypeDef*)malloc(sizeof(USART_TypeDef));

    if (test_case == 1) {
        usart->CR1 = 0;
        usart_enable(usart);
        printf("%d\n", (usart->CR1 & USART_CR1_UE) != 0 ? 1 : 0);
    } else if (test_case == 2) {
        usart->CR1 = 0xFFFF;
        usart_configure_8n1(usart);
        printf("0x%x\n", (unsigned int)(usart->CR1 & (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS)));
    } else if (test_case == 3) {
        usart->CR1 = 0xFFFF;
        usart_configure_8e1(usart);
        int pce_set = (usart->CR1 & USART_CR1_PCE) != 0;
        int m_ps_clear = (usart->CR1 & (USART_CR1_M | USART_CR1_PS)) == 0;
        printf("%d %d\n", pce_set, m_ps_clear);
    } else if (test_case == 4) {
        usart->CR1 = 0;
        usart_configure_9o1(usart);
        uint32_t mask = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS;
        printf("%d\n", (usart->CR1 & mask) == mask ? 1 : 0);
    } else {
        printf("Unknown test case %d\n", test_case);
    }

    free(usart);
    return 0;
}
