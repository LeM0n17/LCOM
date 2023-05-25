#ifndef _LCOM_BULLET_H_
#define _LCOM_BULLET_H_

#include <stdint.h>

#include "object.h"

typedef struct {
    Object *object;
    Object *owner;
    float velocityX;
    float velocityY;
} Bullet;

void freeBullet(Bullet* obj);

#endif
