#include <stdio.h>
#include <stdint.h>

uint16_t read_be16(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[0] << 8) | (uint16_t)buf[1]);
}

uint32_t read_be32(const uint8_t *buf)
{
    return ((uint32_t)buf[0] << 24) |
           ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] <<  8) |
           ((uint32_t)buf[3]);
}

uint16_t read_le16(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[1] << 8) | (uint16_t)buf[0]);
}

uint32_t read_le32(const uint8_t *buf)
{
    return ((uint32_t)buf[3] << 24) |
           ((uint32_t)buf[2] << 16) |
           ((uint32_t)buf[1] <<  8) |
           ((uint32_t)buf[0]);
}
