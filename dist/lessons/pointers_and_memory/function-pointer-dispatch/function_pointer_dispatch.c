#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * TASK 4: Event Dispatch Table with Function Pointers
 */

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
    /* TODO: Iterate dispatch_table and call matching handler:
     *   EventHandler fn = dispatch_table[i]->handler;
     *   fn(data);
     * Return 0 if handled, -1 if unknown event.
     */
    (void)event_id; (void)data;
    return -1;
}
