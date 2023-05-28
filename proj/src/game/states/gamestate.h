#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <stdint.h>
#include <math.h>

#include "../models/list.h"
#include "../models/object.h"
#include "../models/bullet.h"

#include "../../images/Tank_1_Down.xpm"
#include "../../images/Tank_2_Down.xpm"
#include "../../images/Cross.xpm"
#include "../../images/Bullet.xpm"

typedef struct {
    Object *player;
    Object *player_2;
    Object *mouse;
    List *bullets;
    List *walls;
    int bullet_cooldown_2;
    int winner;

} GameState;

GameState* createGame();
void createBullet(GameState *state, uint16_t xOrigin, uint16_t yOrigin, uint16_t xDestination, uint16_t yDestination, Object *owner);
void insertWall(GameState *state, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void gameStep(GameState *state);
void freeGameState();
void process_scancode(GameState* obj, kbd_data_t* data);
int gcd(int a, int b);

#endif // _LCOM_GAME_STATE_H_
