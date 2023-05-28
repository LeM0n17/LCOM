#ifndef _LCOM_CANVAS_H_
#define _LCOM_CANVAS_H_

#include "../../devices/KBC/mouse.h"
#include "../../devices/RTC/rtc.h"
#include "../../devices/video_card/video.h"
#include "../models/object.h"
#include "image.h"
#include "../states/gamestate.h"
#include "../../images/P1Win.xpm"
#include "../../images/P2Win.xpm"

uint32_t walls_color;
uint32_t arena_color;
uint32_t final_color;

void (__canvas__)(uint32_t new_arena_color, uint32_t new_walls_color, uint32_t new_final_color);

int (canvas_draw_arena)();
int (canvas_draw_final)(GameState* state);
int (canvas_draw_object)(Object* obj);
int (canvas_draw_crosshair)(Object* crosshair);
int (draw_walls)(List* list);
int (draw_bullets)(List* list);
int (canvas_refresh)(GameState* game);
int (nightTransform)(uint32_t* color);

#endif // _LCOM_CANVAS_H_
