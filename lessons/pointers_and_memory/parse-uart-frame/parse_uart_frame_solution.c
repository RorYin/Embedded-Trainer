#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t  sof;
    uint8_t  cmd;
    uint16_t length;
    uint32_t payload;
    uint8_t  checksum;
} UartFrame;

UartFrame parse_frame(const uint8_t *raw)
{
    static UartFrame f;
    f.sof      = *raw;
    f.cmd      = *(raw + 1);
    f.length   = (uint16_t)(*(raw + 2)) | ((uint16_t)(*(raw + 3)) << 8);
    f.payload  = (uint32_t)(*(raw + 4)) |
                 ((uint32_t)(*(raw + 5)) << 8) |
                 ((uint32_t)(*(raw + 6)) << 16) |
                 ((uint32_t)(*(raw + 7)) << 24);
    f.checksum = *(raw + 8);
    return f;
}
