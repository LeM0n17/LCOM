#include <lcom/lcf.h>

#include "game.h"

// device variables
int timer_hook_id, kbd_hook_id, mouse_hook_id;
kbd_data_t kbd_data;
mouse_data_t mouse_data;

// game logic variables
GameState *game;
bool draw;

int (game_start)(){
    // create the player
    game = createGame();

    mouse_data.x = mouse_data.prev_x = 640; mouse_data.y = mouse_data.prev_y = 512;

    int flag = video_start(0x11A);
    if (flag) return flag;

    // draw arena
    __canvas__(0xFFF0, 0xF09F);

    flag = canvas_refresh(game);
    if (flag) return flag;
    return video_switch();
}

int (game_loop)(){
    timer_hook_id = 0;
    kbd_hook_id = 1;
    mouse_hook_id = 2;

    int ipc_status;
    message msg;

    // enable timer
    uint8_t timer_bit_no = 0;

    int flag = timer_subscribe_int(&timer_bit_no);
    if (flag) return flag;

    // enable keyboard
    uint8_t kbd_bit_no = 0;

    flag = kbd_subscribe_int(&kbd_bit_no);
    if (flag) return flag;

    // enable mouse
    uint8_t mouse_bit_no;

    mouse_enable_data_report(WAIT);
    if(mouse_subscribe_int(&mouse_bit_no)) return 1;

    uint32_t timer_mask = BIT(timer_bit_no);
    uint32_t mouse_mask = BIT(mouse_bit_no);
    uint32_t kbd_mask = BIT(kbd_bit_no);
    
    while (kbd_data.scancodes[kbd_data.two_byte] != KBD_ESC_BREAKCODE){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool timer_int = msg.m_notify.interrupts & timer_mask;
                bool kbd_int = msg.m_notify.interrupts & kbd_mask;
                bool mouse_int = msg.m_notify.interrupts & mouse_mask;
                draw = 0;  // responsible for knowing whether to draw or not

                if (timer_int) {
                    
                }

                if (kbd_int){
                    kbd_get_scancode(&kbd_data, WAIT);

                    if (kbd_data.error){
                        printf("Error handling keyboard interrupts!\n");
                        return kbd_data.error;
                    }

                    if (!kbd_data.valid) break;

                    //process_scancode(player, &kbd_data);
                    draw = true;
                }

                if (mouse_int){
                    draw = true;
                    mouse_get_data(&mouse_data, WAIT);

                    if (mouse_data.error){
                        printf("Error handling mouse interrupts!\n");
                        return mouse_data.error;
                    }

                    if (mouse_data.packet_no < 3) break;

                    mouse_parse_packet(&mouse_data);
                    mouse_data.packet_no = 0;

                    //test to detect left click
                    if(mouse_data.pp.lb){
                        //video_draw_rectangle(mouse_data.x, mouse_data.y, 50, 50, 0x964b00);
                    }

                }

                if (draw) {
                    flag = canvas_refresh(game);
                    if (flag) return flag;
                    //flag = canvas_refresh_crosshair(&mouse_data);
                    //if (flag) return flag;
                }
            }
            default : break;
        }
    }

    // disable the mouse
    flag = mouse_disable_data_report(WAIT);
    if (flag) return flag;

    flag = mouse_unsubscribe_int();
    if (flag) return flag;

    //printf("%d, %d", mouse_x, mouse_y);

    // disable the keyboard
    flag = kbd_unsubscribe_int();
    if (flag) return flag;

    // disable the timer
    return timer_unsubscribe_int();
}

int game_stop(){
    // delete the game
    freeObject(game->player);
    freeObject(game->mouse);
    freeList(game->bullets, 1);
    freeList(game->walls, 0);
    free(game);

    return video_stop();
}
