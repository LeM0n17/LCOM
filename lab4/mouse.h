#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int (mouse_get_status)(uint8_t *st);
int (mouse_read_buffer)(uint8_t port, uint8_t *out);

#endif
