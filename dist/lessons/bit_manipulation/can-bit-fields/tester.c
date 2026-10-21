#include <stdio.h>
#include <stdint.h>

uint8_t can_get_tx_count(uint32_t status_reg);
uint8_t can_get_rx_count(uint32_t status_reg);
uint8_t can_get_error_counter(uint32_t status_reg);
uint32_t can_set_tx_count(uint32_t status_reg, uint8_t count);

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
    uint32_t can_status = 0x00AB1234;

    if (test_case == 1) {
        printf("0x%x\n", (unsigned int)can_get_tx_count(can_status));
    } else if (test_case == 2) {
        printf("0x%x\n", (unsigned int)can_get_rx_count(can_status));
    } else if (test_case == 3) {
        printf("0x%x\n", (unsigned int)can_get_error_counter(can_status));
    } else if (test_case == 4) {
        uint32_t updated = can_set_tx_count(can_status, 0xFF);
        printf("0x%x\n", (unsigned int)updated);
    } else if (test_case == 5) {
        uint32_t updated = can_set_tx_count(0x12345678, 0x00);
        printf("0x%x\n", (unsigned int)updated);
    } else {
        printf("Unknown test case %d\n", test_case);
    }
    return 0;
}
