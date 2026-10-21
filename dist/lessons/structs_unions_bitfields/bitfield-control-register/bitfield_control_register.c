#include <stdio.h>
#include <stdint.h>

/*
 * TASK 3: Hardware Control Register Bitfield Construction
 *
 * Register Specification:
 *   Bit 0      : PE   (Peripheral Enable, 1=on)
 *   Bit 1      : TXIE (Transmit Interrupt Enable)
 *   Bit 2      : RXIE (Receive Interrupt Enable)
 *   Bits [7:3] : Reserved (must write 0)
 *   Bits [9:8] : SPEED (00=Low, 01=Med, 10=High, 11=Very High)
 *   Bits [31:10]: Reserved (must write 0)
 */

#define CR_PE_SHIFT    0
#define CR_TXIE_SHIFT  1
#define CR_RXIE_SHIFT  2
#define CR_SPEED_SHIFT 8

/* Construct the 32-bit control register raw value from individual fields */
uint32_t build_control_reg(uint8_t pe, uint8_t txie, uint8_t rxie, uint8_t speed)
{
    /* TODO: Assemble control register word */
    (void)pe; (void)txie; (void)rxie; (void)speed;
    return 0;
}

/* Extract the 2-bit speed setting from a 32-bit control register word */
uint8_t get_control_speed(uint32_t reg)
{
    /* TODO: Extract bits [9:8] */
    (void)reg;
    return 0;
}
