#ifndef _LCOM_BULLET_H_
#define _LCOM_BULLET_H_

#include <stdint.h>

#include "object.h"

typedef struct {
    Object *object;
    Object *owner;
    float velocityX;
    float velocityY;
    uint8_t bounces;
} Bullet;

void freeBullet(Bullet* obj);
void bullet_bounce(Bullet* bullet, Object* obj);

#endif
