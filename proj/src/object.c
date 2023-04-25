#include <lcom/lcf.h>

#include "../headers/object.h"

void moveUp(Object* obj, uint16_t speed){
    if(obj->y - speed <= 50){
        obj->y -= (obj->y - 50);
    } else {
        obj->y -= speed;
    }
}

void moveDown(Object* obj, uint16_t speed){
    if(obj->y + 50 + speed >= 974){
        obj->y += (974 - obj->y - 50);
    } else {
        obj->y += speed;
    }
}

void moveLeft(Object* obj, uint16_t speed){
    if(obj->x - speed <= 50){
        obj->x -= (obj->x - 50);
    }else{
        obj->x -= speed;
    }
}

void moveRight(Object* obj, uint16_t speed){
    if(obj->x + 50 + speed >= 1230){
        obj->x += (1230 - obj->x - 50);
    } else {
        obj->x += speed;
    }
}

void process_scancode(Object* obj, uint8_t scan_codes[]){
    if((scan_codes[0] == 0xe0 && scan_codes[1] == 0x48) || (scan_codes[0] == 0xe0 && scan_codes[1] == 0x11)){
        moveUp(obj,10);
    }
    else if((scan_codes[0] == 0xe0 && scan_codes[1] == 0x4b) || (scan_codes[0] == 0xe0 && scan_codes[1] == 0x1e)){
        moveLeft(obj,10);
    }
    else if((scan_codes[0] == 0xe0 && scan_codes[1] == 0x50) || (scan_codes[0] == 0xe0 && scan_codes[1] == 0x1f)){
        moveDown(obj,10);
    }
    else if((scan_codes[0] == 0xe0 && scan_codes[1] == 0x4d) || (scan_codes[0] == 0xe0 && scan_codes[1] == 0x20)){
        moveRight(obj,10);
    }
}
