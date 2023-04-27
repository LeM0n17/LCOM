#include "defines.h"
#include <stdint.h>
#include <lcom/lcf.h>

int(change_mode)(uint16_t mode);

int(map_vm)(uint16_t mode);

int(normalize_color)(uint32_t color, uint32_t *new_color);

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
