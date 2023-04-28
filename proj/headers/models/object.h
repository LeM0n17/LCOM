#ifndef _LCOM_OBJECT_H_
#define _LCOM_OBJECT_H_

#include <stdint.h>

#pragma pack(1)

typedef struct {
    uint16_t x;
    uint16_t y;
} Object;

#pragma options align=reset

void moveUp(Object* obj, uint16_t speed);

void moveDown(Object* obj, uint16_t speed);

void moveLeft(Object* obj, uint16_t speed);

void moveRight(Object* obj, uint16_t speed);

void process_scancode(Object* obj, uint8_t scan_codes[]);

#endif // _LCOM_OBJECT_H_
