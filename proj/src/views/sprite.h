#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>

typedef struct {
    uint16_t width, height;
    uint32_t* colors;
} Sprite;

Sprite* sprite_create(xpm_map_t xpm);
void sprite_destroy(Sprite* sprite);

#endif // _LCOM_SPRITE_H_
