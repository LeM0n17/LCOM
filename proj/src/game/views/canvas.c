#include <lcom/lcf.h>

#include "canvas.h"

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

int (canvas_draw_object)(Object* obj){
    printf("0x%x\n", *obj->image->colors);
    return (obj->image->type == SPRITE) ? video_draw_sprite(obj->x, obj->y, obj->width, obj->height, obj->image->colors)
                                        : video_draw_rectangle(obj->x, obj->y, obj->width, obj->height, *obj->image->colors);
}

int (canvas_refresh)(Object* obj){
    if (obj->x == obj->prev_x && obj->y == obj->prev_y) return 0;

    int flag = video_draw_rectangle(obj->prev_x, obj->prev_y, obj->width, obj->height, arena_color);
    if (flag) return flag;

    flag = canvas_draw_object(obj);
    if (flag) return flag;

    return video_switch();
}
