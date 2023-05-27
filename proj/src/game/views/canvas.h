#ifndef _LCOM_CANVAS_H_
#define _LCOM_CANVAS_H_

#include "../../devices/video_card/video.h"
#include "../models/object.h"
#include "../models/gamestate.h"
#include "../../devices/KBC/mouse.h"
#include "../../devices/RTC/rtc.h"

uint32_t walls_color;
uint32_t arena_color;

void (__canvas__)(uint32_t new_arena_color, uint32_t new_walls_color);

int (canvas_draw_arena)();
int (canvas_draw_object)(Object* obj);
int (canvas_draw_crosshair)(Object* crosshair);
int (draw_walls)(List* list);
int (draw_bullets)(List* list);
int (canvas_refresh)(GameState* game);
int (nightTransform)(uint32_t* color);

#endif // _LCOM_CANVAS_H_
