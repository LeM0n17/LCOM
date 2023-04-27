#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

void (kbc_ih)();

int (kbc_get_status)(uint8_t *st);

int (kbc_read_out_buffer)(uint8_t *out);

int (kbc_subscribe_int)(uint8_t *bit_no);

int (kbc_unsubscribe_int)();

int (kbc_print)();

#endif
