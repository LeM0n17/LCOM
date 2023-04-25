#include <lcom/lcf.h>

#include "../headers/object.h"

void moveUp(Object obj, uint16_t speed){
    obj.y -= speed;
}

void moveDown(Object obj, uint16_t speed){
    obj.y += speed;
}

void moveLeft(Object obj, uint16_t speed){
    obj.x -= speed;
}

void moveRight(Object obj, uint16_t speed){
    obj.x += speed;
}