#include <stdio.h>

#include "list.h"
#include "../headers/keyboard.h"
#include "../headers/video.h"
#include "../headers/object.h"


extern uint8_t data;
extern int process;
extern uint8_t scan_codes[2];

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

int kbd_loop(Object player){
    int r = 0;
    uint8_t bit = 0;
    if(kbc_subscribe_int(&bit)) return 1;
    uint32_t kanna = BIT(bit);
    
    int ipc_status;
    message msg;
 
    while(data != KBD_ESC) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & kanna) { /* subscribed interrupt */
                        if(kbc_read_out_buffer(&data)) return 1;
                        kbc_ih();
                        if(process) process_scancode(player, scan_codes);
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if(kbc_unsubscribe_int()){
        return 1;
    }
    return 0;

}



int(proj_main_loop)(){
    Object player;
    player.x = 100;
    player.y = 100;
    if(vg_init(0x11A) == NULL){
        vg_exit();
        return 1;
    }
    //draw arena
    vg_draw_arena();

    if(kbd_loop(player)){
        vg_exit();
        return 1;
    }
    vg_exit();
    return 0;
}
