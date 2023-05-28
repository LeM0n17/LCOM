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

    menu->start_button = start_button;

    // create the quit button
    Object *quit_button = malloc(sizeof *quit_button);

    quit_button->image = image_create_sprite((xpm_map_t) Cross_xpm);
    quit_button->x = 640;
    quit_button->y = 500;

    menu->quit_button = quit_button;

    return menu;  
}

void menu_step(menu_state *state){
    Button* start_button = state->start_button;
    Button* quit_button = state->quit_button;

    // check if the cursor is hovering over the start button
    bool collides = checkCollisions(state->cursor, state->start_button);

    if (colides && !start_button->hover) {
        start_button->hover = true;

        image_destroy(start_button->image);
        start_button->image = image_create_shape((xpm_image_t) Start_hover.xpm);
    }
    else if (!collides && start->button->hover) {
        start_button->hover = false;

        image_destroy(start_button->image);
        start_button->image = image_create_shape((xpm_image_t) Start.xpm);
    }

    // check if the cursor is hovering over the quit button
    collides = checkCollisions(state->cursor, state->quit_button);

    if (colides && !quit_button->hover) {
        quit_button->hover = true;

        image_destroy(quit_button->image);
        quit_button->image = image_create_shape((xpm_image_t) Quit_hover.xpm);
    }
    else if (!collides && quit_button->hover) {
        quit_button->hover = false;

        image_destroy(quit_button->image);
        quit_button->image = image_create_shape((xpm_image_t) Quit.xpm);
    }
}

void menu_destroy(menu_state* state) {
    // delete the start button
    free(state->start_button->body);
    free(state->start_button);

    // delete the quit button
    free(state->quit_button->body);
    free(state->quit_button);

    free(state);
}
