#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

void(kbc_ih)();

int(kbc_subscribe_int)(uint8_t *bit_no);

int(kbc_unsubscribe_int)();

int(kbc_get_status)(uint8_t *st);

int(kbc_read_out_buffer)(uint8_t *data);

int(kbc_print_codes)();

int(kbc_send_cmd)(uint8_t port, uint8_t cmd);

int(kbc_reenable_int)();

int check_status(uint8_t st);

#endif
