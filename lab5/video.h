#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include "VBE_macros.h"
#include <lcom/lcf.h>

void* (vg_init)(uint16_t mode);
int (vg_set_mode)(uint16_t mode);
void privctl(phys_bytes adress, size_t size);

void (vg_draw_hlin)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t len, uint16_t height, uint32_t color);
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
int (vg_update)(xpm_map_t xpm, uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y);

#endif
