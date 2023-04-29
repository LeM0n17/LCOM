#include <stdio.h>

/* DEVICES */
#include "devices/KBC/keyboard.h"
#include "devices/video_card/video.h"

/* MODELS */
#include "models/object.h"

/* VIEWS */
#include "views/canvas.h"

#define WAIT 5

int kbd_hook_id;
bool kbd_ih_error;
kbd_data data;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("./trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("./output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int disable_video(int flag){
    vg_exit();
    return flag;
}

int proj_int_loop(Object* player){
    // global variables
    kbd_hook_id = 0;

    // local variables
    int ipc_status;
    message msg;

    uint8_t kbd_bit_no = 0;
    int flag = kbd_subscribe_int(&kbd_bit_no);
    if (flag) return flag;

    uint32_t kbd_mask = BIT(kbd_bit_no);

    uint16_t old_x = player->x;
    uint16_t old_y = player->y;
    
    while (data.scancode[data.two_byte] != KBD_ESC_BREAKCODE){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool kbd_int = msg.m_notify.interrupts & kbd_mask;
                if (!kbd_int) break;

                kbd_get_scancode(&data, WAIT);

                if (kbd_ih_error) return kbd_ih_error;
                if (!data.valid) break;

                process_scancode(player, &data);

                flag = canvas_refresh(player, old_x, old_y);
                if (flag) return flag;

                old_x = player->x; old_y = player->y;
            }
            default : break;
        }
    }

    return kbd_unsubscribe_int();
}

int (proj_main_loop)(){
    Object player = {100, 100};

    int flag = video_start(0x11A);
    if (flag) return disable_video(flag);

    // draw arena
    flag = canvas_draw_arena(0xFFF0, 0xF09F);
    if (flag) return disable_video(flag);

    // draw player
    flag = video_draw_rectangle(player.x, player.y, 50, 50, 0x000F);
    if (flag) return disable_video(flag);

    flag = proj_int_loop(&player);
    if (flag) return disable_video(flag);

    return vg_exit();
}
