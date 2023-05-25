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
    return (obj->image->type == SPRITE) ? video_draw_sprite(obj->x, obj->y, obj->width, obj->height, obj->image->colors)
                                        : video_draw_rectangle(obj->x, obj->y, obj->width, obj->height, *obj->image->colors);
}

int (canvas_draw_crosshair)(Object* crosshair){
    return video_draw_sprite(crosshair->x + crosshair->width/2, crosshair->y + crosshair->height/2, 
        crosshair->width, crosshair->height, crosshair->image->colors);
}

int (canvas_refresh)(GameState* game){
    int flag = canvas_draw_arena();
    if (flag) return flag;

    flag = canvas_draw_object(game->player);
    if (flag) return flag;

    flag = canvas_draw_crosshair(game->mouse);
    if (flag) return flag;

    return video_switch();
}
