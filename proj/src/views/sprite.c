#include "sprite.h"

Sprite* sprite_create(xpm_map_t xpm){
    Sprite* sprite = (Sprite*) malloc(sizeof *sprite);
    if (sprite == NULL) return NULL;

    // load the XPM
    xpm_image_t image;

    sprite->colors = xpm_load(xpm, XPM_8_8_8, &image);
    sprite->width = image.width; sprite.height = image.width;

    return sprite;
}

void destroy_sprite(Sprite* sprite){
    if (sprite) free(sprite->colors);
    free(sprite);
}
