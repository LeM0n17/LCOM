#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>

int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int(mouse_get_status)();
int(check_status)(uint8_t st);
void(mouse_ih)();
int(mouse_read_packet)();
void(parse_packets)();

#endif
