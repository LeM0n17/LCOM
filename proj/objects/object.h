#ifndef _LCOM_OBJECT_H_
#define _LCOM_OBJECT_H_

#include <stdint.h>

typedef struct Object
{
    uint16_t x;
    uint16_t y;
};


void moveUp(Object& obj, uint16_t speed){
    obj.y -= speed;
}

void moveDown(Object& obj, uint16_t speed){
    obj.y += speed;
}

void moveLeft(Object& obj, uint16_t speed){
    obj.x -= speed;
}

void moveRight(Object& obj, uint16_t speed){
    obj.x += speed;
}


#endif

