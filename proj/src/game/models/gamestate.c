#include <lcom/lcf.h>

#include "gamestate.h"


GameState* createGame() {
    Object* plr = malloc(sizeof(* plr));
    plr->x = 100;
    plr->y = 100;
    plr->width = plr->height = 64;
    plr->image = image_create_sprite((xpm_map_t) Tank_1_Down_xpm);

    Object* mouse = malloc(sizeof(* mouse));
    mouse->image = image_create_sprite((xpm_map_t) Cross_xpm);
    mouse->width = mouse->height = 32;
    mouse->x = 640;
    mouse->y = 512;

    GameState* state =  malloc(sizeof(* state));
    state->bullets = create_list();
    state->walls = create_list();
    state->player = plr;
    state->mouse = mouse;

    return state;
} 

void insertWall(GameState *state, Object *wall) {
    push_back(state->walls, wall);
}

void gameStep(GameState *state) {
    return;
}

void createBullet(GameState *state, uint16_t xOrigin, uint16_t yOrigin, uint16_t xDestination, uint16_t yDestination, Object *owner) {
    uint16_t xDifference = xDestination - xOrigin;
    uint16_t yDifference = yDestination - yOrigin;

    // Calculate Unit Vector
    float distance = sqrtf(xDifference * xDifference + yDifference * yDifference);

    Object *object = malloc(sizeof(* object));
    object->x = xOrigin;
    object->y = yOrigin;
    object->height = object->width = 16;

    Bullet *bullet = malloc(sizeof(* bullet));
    bullet->owner = owner;
    bullet->velocityX = 10 * (xDifference / distance);
    bullet->velocityY = 10 * (yDifference / distance);
    bullet->object = object;

    push_back(state->bullets, bullet);
}
