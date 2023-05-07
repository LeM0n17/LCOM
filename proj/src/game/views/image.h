#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>

/**
 * @brief struct that models an image
 * @param type specifies whether the image is a sprite or a simple vectorial shape
 * @param colors contains the color(s) of the image
 */
typedef struct {
    enum {SPRITE, SHAPE} type;
    uint32_t *colors;
} Image;

Image *image_create_sprite(xpm_map_t xpm);
Image *image_create_shape(uint32_t color);
void image_destroy(Image* image);

#endif // _LCOM_SPRITE_H_