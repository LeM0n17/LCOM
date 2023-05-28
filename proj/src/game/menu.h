#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

/* DEVICES */
#include "../devices/KBC/keyboard.h"
#include "../devices/video_card/video.h"
#include "../devices/KBC/mouse.h"
#include "../devices/timer/timer.h"

/* MODELS */
#include "models/object.h"

/* VIEWS */
#include "views/canvas.h"

/* STATES */
#include "states/menustate.h"

#include "game.h"

#define WAIT 5

/**
 * @brief initializes all aspects of the menu, including allocating memory for its objects
 * @return zero on success, non-zero otherwise
 */
int (menu_start)();

/**
 * @brief subscribes the interrupts of each device and executes the interrupt loop
 * @return zero on success, non-zero otherwise 
 */
int (menu_loop)();

/**
 * @brief terminates the program, ensuring all allocated memory is freed
 * @return zero on success, non-zero otherwise
 */
int (menu_stop)();

#endif // _LCOM_MENU_H_
