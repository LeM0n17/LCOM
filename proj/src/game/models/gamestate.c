#include <lcom/lcf.h>

#include "gamestate.h"


GameState* createGame() {
    // CREATE PLAYER 1
    Object* plr = malloc(sizeof(* plr));
    plr->x = 178;
    plr->y = 50;
    plr->width = plr->height = 64;
    plr->image = image_create_sprite((xpm_map_t) Tank_1_Down_xpm);

    //CREATE PLAYER 2
    Object* plr_2 = malloc(sizeof(* plr_2));
    plr_2->x = 178;
    plr_2->y = 200;
    plr_2->width = plr_2->height = 64;
    plr_2->image = image_create_sprite((xpm_map_t) Tank_1_Down_xpm);

    // CREATE MOUSE
    Object* mouse = malloc(sizeof(* mouse));
    mouse->image = image_create_sprite((xpm_map_t) Cross_xpm);
    mouse->width = mouse->height = 32;
    mouse->x = 640;
    mouse->y = 512;

    // CREATE FINAL GAME
    GameState* state =  malloc(sizeof(* state));
    state->bullets = create_list();
    state->walls = create_list();
    state->player = plr;
    state->player_2 = plr_2;
    state->mouse = mouse;
    state->bullet_cooldown_2 = 0;

    // CREATE WALLS
    
    insertWall(state, 0, 0, 178, 1024); // L
    insertWall(state, 1280 - 178, 0, 178, 1024); // R
    insertWall(state, 0, 0, 1280, 50); // T
    insertWall(state, 0, 1024 - 50, 1280, 50); // B 

    return state;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

void insertWall(GameState *state, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    Object* wall = malloc(sizeof* wall);
    wall->x = x;
    wall->y = y;
    wall->width = width;
    wall->height = height;
    wall->image = image_create_shape(width, height, 0x03F0);

    push_back(state->walls, wall);
}

void gameStep(GameState *state) {
    Bullet *bullet = NULL;
    ListElement* ptr = state->bullets->head;
    ListElement* prev = NULL;
    ListElement* element = state->walls->head;
    Object *value = NULL;

    while (ptr != NULL) {
        element = state->walls->head;
        bullet = ptr->value;
        prev = ptr;
        ptr = ptr->next;

        bullet->object->x += bullet->velocityX;
        bullet->object->y += bullet->velocityY;
        
        bool deleted = false;
        while (!deleted && (element != NULL)) {
            value = element->value;
            element = element->next;
            if (pointInObject(value, bullet->object->x, bullet->object->y)){
                pop_element(state->bullets, prev, 1);
                deleted = true;
            }
        }
    }
}

void createBullet(GameState *state, uint16_t xOrigin, uint16_t yOrigin, uint16_t xDestination, uint16_t yDestination, Object *owner) {
    int16_t xDifference = xDestination - xOrigin;
    int16_t yDifference = yDestination - yOrigin;

    // Calculate Unit Vector
    float distance = sqrtf(xDifference * xDifference + yDifference * yDifference);

    Object *object = malloc(sizeof(* object));
    object->x = xOrigin;
    object->y = yOrigin;
    object->height = object->width = 16;
    object->image = image_create_sprite((xpm_map_t) Bullet_xpm);

    Bullet *bullet = malloc(sizeof(* bullet));
    bullet->owner = owner;
    bullet->velocityX = 10 * (xDifference / distance);
    bullet->velocityY = 10 * (yDifference / distance);
    bullet->object = object;

    push_back(state->bullets, bullet);
}

void process_scancode(GameState *state, kbd_data_t* data){
    uint8_t* scancodes = data->scancodes;
    uint16_t cacheY = state->player->y;
    uint16_t cacheX = state->player->x;
    uint16_t cacheY_2 = state->player_2->y;
    uint16_t cacheX_2 = state->player_2->x;

    if (MOVE_UP_2(scancodes)){
        moveUp(state->player, 5);
    }
    else if (MOVE_DOWN_2(scancodes)){
        moveDown(state->player, 5);
    }
    else if (MOVE_LEFT_2(scancodes)){
        moveLeft(state->player, 5);
    }
    else if (MOVE_RIGHT_2(scancodes)){
        moveRight(state->player, 5);
    }

    if (MOVE_UP(scancodes)){
        moveUp(state->player_2, 5);
    }
    else if (MOVE_DOWN(scancodes)){
        moveDown(state->player_2, 5);
    }
    else if (MOVE_LEFT(scancodes)){
        moveLeft(state->player_2, 5);
    }
    else if (MOVE_RIGHT(scancodes)){
        moveRight(state->player_2, 5);
    }

    if(state->bullet_cooldown_2 > 30){
        if(SHOOT_RIGHT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x +state->player_2->width, state->player_2->y + state->player_2->height/2, state->player_2);
        } else if(SHOOT_LEFT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x, state->player_2->y + state->player_2->height/2, state->player_2);
        } else if(SHOOT_UP(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x +state->player_2->width / 2, state->player_2->y, state->player_2);
        } else if(SHOOT_DOWN(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x +state->player_2->width / 2, state->player_2->y + state->player_2->height, state->player_2);
        } else if(SHOOT_DOWN_LEFT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x, state->player_2->y + state->player_2->height, state->player_2);
        } else if(SHOOT_UP_LEFT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x, state->player_2->y, state->player_2);
        } else if(SHOOT_DOWN_RIGHT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x +state->player_2->width, state->player_2->y + state->player_2->height, state->player_2);
        } else if(SHOOT_UP_RIGHT(scancodes)){
            createBullet(state, state->player_2->x + state->player_2->width / 2, state->player_2->y + state->player_2->height / 2, state->player_2->x + state->player_2->width, state->player_2->y, state->player_2);
        }
        state->bullet_cooldown_2 = 0;
    }

    if(checkCollisions(state->player, state->player_2)){
        state->player->y = cacheY;
        state->player->x = cacheX;
    }

    if (checkCollisions(state->player_2, state->player)) {
        state->player_2->y = cacheY_2;
        state->player_2->x = cacheX_2;
    }

    Object* wall = NULL;
    ListElement* ptr = state->walls->head;
    while (ptr != NULL) { // COLLISION CHECKING 
        wall = ptr->value;
        ptr = ptr->next;
        if (checkCollisions(state->player, wall)) {
            state->player->y = cacheY;
            state->player->x = cacheX;
        }
    }

    wall = NULL;
    ptr = state->walls->head;
    while (ptr != NULL) { // COLLISION CHECKING 
        wall = ptr->value;
        ptr = ptr->next;
        if (checkCollisions(state->player_2, wall)) {
            state->player_2->y = cacheY_2;
            state->player_2->x = cacheX_2;
        }
    }

}
