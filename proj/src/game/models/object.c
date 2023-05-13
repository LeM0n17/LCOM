#include <lcom/lcf.h>

#include "object.h"

void moveUp(Object* obj, uint16_t speed){
    if(obj->y - speed <= 50){
        obj->y -= (obj->y - 64);
    } else {
        obj->y -= speed;
    }
}

void moveDown(Object* obj, uint16_t speed){
    if (obj->y + (obj->height) + speed >= 974){
        obj->y += (974 - obj->y - 64);
    }
    else {
        obj->y += speed;
    }
}

void moveLeft(Object* obj, uint16_t speed){
    if (obj->x - speed <= 50){
        obj->x -= (obj->x - 64);
    }
    else {
        obj->x -= speed;
    }
}

void moveRight(Object* obj, uint16_t speed){
    if (obj->x + (obj->width) + speed >= 1230){
        obj->x += (1230 - obj->x - 64);
    }
    else {
        obj->x += speed;
    }
}

void process_scancode(Object* obj, kbd_data_t* data){
    uint8_t* scancodes = data->scancodes;

    if (MOVE_UP(scancodes)){
        moveUp(obj, 5);
    }
    else if (MOVE_DOWN(scancodes)){
        moveDown(obj, 5);
    }
    else if (MOVE_LEFT(scancodes)){
        moveLeft(obj, 5);
    }
    else if (MOVE_RIGHT(scancodes)){
        moveRight(obj, 5);
    }
}
