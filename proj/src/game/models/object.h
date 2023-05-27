#ifndef _LCOM_OBJECT_H_
#define _LCOM_OBJECT_H_

#include <stdint.h>

#include "../../devices/KBC/keyboard.h"
#include "../views/image.h"

#define MOVE_UP(scancodes) (scancodes[0] == 0x11)
#define MOVE_DOWN(scancodes) (scancodes[0] == 0x1f)
#define MOVE_LEFT(scancodes) (scancodes[0] == 0x1e)
#define MOVE_RIGHT(scancodes) (scancodes[0] == 0x20)

#define MOVE_UP_2(scancodes) (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x48)
#define MOVE_DOWN_2(scancodes) (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x50)
#define MOVE_LEFT_2(scancodes) (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4b)
#define MOVE_RIGHT_2(scancodes) (scancodes[0] == KBD_2B_SCANCODE && scancodes[1] == 0x4d)

#define SHOOT_RIGHT(scancodes) (scancodes[0] == 0x4d)
#define SHOOT_LEFT(scancodes) (scancodes[0] == 0x4b)
#define SHOOT_UP(scancodes) (scancodes[0] == 0x48)
#define SHOOT_DOWN(scancodes) (scancodes[0] == 0x50)
#define SHOOT_DOWN_RIGHT(scancodes) (scancodes[0] == 0x51)
#define SHOOT_UP_RIGHT(scancodes) (scancodes[0] == 0x49)
#define SHOOT_DOWN_LEFT(scancodes) (scancodes[0] == 0x4f)
#define SHOOT_UP_LEFT(scancodes) (scancodes[0] == 0x47)

/**
 * @brief struct that models a collidable game element, that is, an element with a hitbox
 * @param x the x-coordinate of the Object in the current frame
 * @param y the y-coordinate of the Object in the current frame
 * @param prev_x the x-coordinate of the Object in the previous frame
 * @param prev_y the y-coordinate of the Object in the previous frame
 * @param width the width of the Object
 * @param height the height of the Object
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

void freeObject(Object *obj);
bool pointInObject(Object *obj, uint16_t x, uint16_t y);
bool checkCollisions(Object *obj1, Object* obj2);

#endif // _LCOM_OBJECT_H_
