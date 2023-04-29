#ifndef _LCOM_OBJECT_H_
#define _LCOM_OBJECT_H_

#include <stdint.h>

#include "../devices/KBC/keyboard.h"

#define MOVE_UP(scancodes) (scancodes[0] == 0x11 || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x48))
#define MOVE_DOWN(scancodes) (scancodes[0] == 0x1f || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x50))
#define MOVE_LEFT(scancodes) (scancodes[0] == 0x1e || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4b))
#define MOVE_RIGHT(scancodes) (scancodes[0] == 0x20 || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4d))

typedef struct {
    uint16_t x;
    uint16_t y;
} Object;

void moveUp(Object* obj, uint16_t speed);
void moveDown(Object* obj, uint16_t speed);
void moveLeft(Object* obj, uint16_t speed);
void moveRight(Object* obj, uint16_t speed);

void process_scancode(Object* obj, kbd_data* data);

#endif // _LCOM_OBJECT_H_
