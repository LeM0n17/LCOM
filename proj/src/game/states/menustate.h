#ifndef _LCOM_MENU_STATE_H_
#define _LCOM_MENU_STATE_H_

#include "../models/list.h"
#include "../models/object.h"

typedef struct {
    Object* body;
    bool hover;
} Button;

typedef struct {
    Button* start_button;
    Button* quit_button;
    Object* cursor;
} menu_state;

menu_state* __menu__();
void menu_step(menu_state *state);
void menu_destroy();

#endif // _LCOM_MENU_STATE_H_
