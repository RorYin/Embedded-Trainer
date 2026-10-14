#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef void (*EventHandler)(uint32_t data);

typedef struct {
    uint32_t     event_id;
    EventHandler handler;
} EventEntry;

static int g_btn_count = 0;
static int g_tmr_count = 0;
static int g_uart_count = 0;

static void handle_button(uint32_t data) { g_btn_count += (int)data; }
static void handle_timer(uint32_t data)  { g_tmr_count += (int)data; }
static void handle_uart(uint32_t data)   { g_uart_count += (int)data; }

static EventEntry *dispatch_table[3];

void init_dispatch_table(void)
{
    dispatch_table[0] = (EventEntry*)malloc(sizeof(EventEntry));
    dispatch_table[0]->event_id = 0x01; dispatch_table[0]->handler = handle_button;

    dispatch_table[1] = (EventEntry*)malloc(sizeof(EventEntry));
    dispatch_table[1]->event_id = 0x02; dispatch_table[1]->handler = handle_timer;

    dispatch_table[2] = (EventEntry*)malloc(sizeof(EventEntry));
    dispatch_table[2]->event_id = 0x03; dispatch_table[2]->handler = handle_uart;
}

int dispatch_event(uint8_t event_id, uint32_t data)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (dispatch_table[i]->event_id == event_id) {
            EventHandler fn = dispatch_table[i]->handler;
            fn(data);
            return 0;
        }
    }
    return -1;
}

int main(void)
{
    init_dispatch_table();

    if (dispatch_event(0x01, 5) != 0) { printf("Failed dispatch 0x01\n"); return 1; }
    if (g_btn_count != 5) { printf("Failed button count\n"); return 2; }

    if (dispatch_event(0x02, 10) != 0) { printf("Failed dispatch 0x02\n"); return 3; }
    if (g_tmr_count != 10) { printf("Failed timer count\n"); return 4; }

    if (dispatch_event(0x03, 1) != 0) { printf("Failed dispatch 0x03\n"); return 5; }
    if (g_uart_count != 1) { printf("Failed uart count\n"); return 6; }

    if (dispatch_event(0x99, 100) != -1) { printf("Failed unknown event check\n"); return 7; }

    free(dispatch_table[0]);
    free(dispatch_table[1]);
    free(dispatch_table[2]);

    printf("All tests passed!\n");
    return 0;
}
