#include <stdio.h>
#include <stdint.h>

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
    int c = read_case_num();
    uint8_t frame[6];

    if (c == 1) {
        build_command_frame(0x01, 0x1234, frame);
        printf("frame: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x, valid: %d\n",
               frame[0], frame[1], frame[2], frame[3], frame[4], frame[5],
               validate_command_frame(frame));
    } else if (c == 2) {
        build_command_frame(0xA5, 0x0000, frame);
        printf("frame: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x, valid: %d\n",
               frame[0], frame[1], frame[2], frame[3], frame[4], frame[5],
               validate_command_frame(frame));
    } else if (c == 3) {
        build_command_frame(0x01, 0x1234, frame);
        frame[4] = frame[4] ^ 0xFF;
        printf("valid: %d\n", validate_command_frame(frame));
    } else if (c == 4) {
        build_command_frame(0x01, 0x1234, frame);
        frame[0] = 0x00;
        printf("valid: %d\n", validate_command_frame(frame));
    } else if (c == 5) {
        build_command_frame(0x01, 0x1234, frame);
        frame[5] = 0xFF;
        printf("valid: %d\n", validate_command_frame(frame));
    }
    return 0;
}
