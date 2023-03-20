#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "keyboard.h"
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>

extern uint8_t data;
extern int count;
extern int cnt;

uint8_t scan_codes[2];
int i = 0;

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

int(kbd_test_scan)() {
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
                        if(kbc_print_codes()) return 1;
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
    if(kbd_print_no_sysinb(count)){
        return 1;
    }
    return 0;
}



int(kbd_test_poll)() {
    while(data != KBD_ESC){
        if(kbc_read_out_buffer(&data)){
            if(tickdelay(micros_to_ticks(DELAY_US))){
                return 1;
            }
        }
        kbc_ih();
        if(kbc_print_codes()) return 1;
    }

    if(kbc_reenable_int()) return 1;

    if(kbd_print_no_sysinb(count)) return 1;

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
    uint8_t idle = n;
    uint8_t timer_bit = 0;
    uint8_t kbd_bit = 0;
    int r = 0;
    if(timer_subscribe_int(&timer_bit)) return 1;
    if(kbc_subscribe_int(&kbd_bit)) return 1;
    uint32_t timer_int = BIT(timer_bit);
    uint32_t kbd_int = BIT(kbd_bit);

    int ipc_status;
    message msg;
    while(data != KBD_ESC && idle){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:{
                    if(msg.m_notify.interrupts & timer_int){
                        timer_int_handler();
                        if(cnt % 60 == 0){
                            idle--;
                        }
                    }
                    if(msg.m_notify.interrupts & kbd_int){
                        if(kbc_read_out_buffer(&data)) return 1;
                        kbc_ih();
                        idle = n;
                        cnt = 0;
                        if(kbc_print_codes()) return 1;
                    }
                    break;
                }
                default:
                    break;
            }

        }
    }
    if(kbc_unsubscribe_int()) return 1;
    return timer_unsubscribe_int();
}

