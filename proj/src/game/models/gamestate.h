#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <stdint.h>
#include <math.h>

#include "list.h"
#include "object.h"
#include "bullet.h"

#include "../../images/Tank_1_Down.xpm"
#include "../../images/Cross.xpm"

typedef struct {
    Object *player;
    Object *mouse;
    List *bullets;
    List *walls;

} GameState;

GameState* createGame();
void createBullet(GameState *state, uint16_t xOrigin, uint16_t yOrigin, uint16_t xDestination, uint16_t yDestination, Object *owner);
void insertWall(GameState *state, Object *wall);
void gameStep(GameState *state);
void freeGameState();

#endif // _LCOM_GAME_STATE_H_
