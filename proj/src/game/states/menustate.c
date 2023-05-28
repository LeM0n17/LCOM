#include <lcom/lcf.h>

#include "menustate.h"

menu_state* __menu__() {
    menu_state *menu = malloc(sizeof* menu);

    // create the cursor
    Object* cursor = malloc(sizeof *cursor);
    cursor->image = image_create_sprite((xpm_map_t) Cross_xpm);
    cursor->width = cursor->height = 32;
    cursor->x = 640;
    cursor->y = 512;

    menu->cursor = cursor;

    // create the start button
    Object *start_button = malloc(sizeof *start_button);

    start_button->image = image_create_sprite((xpm_map_t) Cross_xpm);
    start_button->x = 640;
    start_button->y = 500;

    push_back(menu->buttons, start_button);

    // create the quit button
    Object *quit_button = malloc(sizeof *quit_button);

    quit_button->image = image_create_sprite((xpm_map_t) Cross_xpm);
    quit_button->x = 640;
    quit_button->y = 500;

    push_back(menu->buttons, quit_button);    
}

void menu_step(menu_state *state){
    List* list = state->buttons;

    for (ListElement* it = list->head; it != list->tail; it = it->next){
        Object *button = (Object*) it->value;

        if (!checkCollisions(button, state->cursor))
            continue;
        
        image_destroy(button->image);
        button->image = image_create_sprite();
    }
}


