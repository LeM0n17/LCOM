#ifndef _LCOM_OBJECT_H_
#define _LCOM_OBJECT_H_

#include <stdint.h>

#include "../../devices/KBC/keyboard.h"
#include "../views/image.h"

#define MOVE_UP(scancodes) (scancodes[0] == 0x11 || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x48))
#define MOVE_DOWN(scancodes) (scancodes[0] == 0x1f || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x50))
#define MOVE_LEFT(scancodes) (scancodes[0] == 0x1e || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4b))
#define MOVE_RIGHT(scancodes) (scancodes[0] == 0x20 || (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4d))

/**
 * @brief struct that models a collidable game element, that is, an element with a hitbox
 * @param x the x-coordinate of the Object
 * @param y the y-coordinate of the Object
 * @param image image that represents the visual appearance of the Object
 */
typedef struct {
    uint16_t x, y;
    uint16_t width, height;
    Image* image;
} Object;

void moveUp(Object* obj, uint16_t speed);
void moveDown(Object* obj, uint16_t speed);
void moveLeft(Object* obj, uint16_t speed);
void moveRight(Object* obj, uint16_t speed);

void process_scancode(Object* obj, kbd_data_t* data);

#endif // _LCOM_OBJECT_H_