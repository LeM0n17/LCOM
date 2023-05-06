#include <lcom/lcf.h>

#include "canvas.h"

uint32_t walls_color;
uint32_t arena_color;

void __canvas__(uint32_t new_arena_color, uint32_t new_walls_color){
    arena_color = new_arena_color;
    walls_color = new_walls_color;
}

int (canvas_draw_arena)(){
    return video_draw_rectangle(0, 0, 1230, 50, walls_color) ||
        video_draw_rectangle(1230, 0, 50, 1024, walls_color) ||
        video_draw_rectangle(0, 0, 50, 1024, walls_color) ||
        video_draw_rectangle(50, 974, 1180, 50, walls_color) ||
        video_draw_rectangle(50, 50, 1180, 924, arena_color);
}

int (canvas_refresh)(Object* obj, uint16_t old_x, uint16_t old_y){
    if (obj->x == old_x && obj->y == old_y) return 0;

    int flag = video_draw_rectangle(old_x, old_y, 50, 50, arena_color);
    if (flag) return flag;

    flag = video_draw_rectangle(obj->x, obj->y, 50, 50, 0x000F);
    if (flag) return flag;

    return video_switch();
}
