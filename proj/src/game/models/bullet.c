#include <lcom/lcf.h>

#include <math.h>
#include "bullet.h"

void freeBullet(Bullet* obj) {
    free(obj->object);
    free(obj);
}
