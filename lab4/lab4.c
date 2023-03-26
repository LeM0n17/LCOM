// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
extern bool ready;
extern struct packet pp;
extern int count;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no;
    mouse_enable_data_reporting();
    if(mouse_subscribe_int(&bit_no)) return 1;
    uint32_t irq_set = BIT(bit_no);

    int ipc_status;
    message msg;

     while (cnt != 0) {

    int r;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {

        case HARDWARE: {

          if (msg.m_notify.interrupts & irq_set) {

            mouse_ih();
            if (ready) {
              ready = false;
              mouse_print_packet(&pp);
              cnt--;
            }
            
          }

          break;
        }

        default: break;
      }
    }
  }
  mouse_unsubscribe_int();

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;
  mouse_enable_data_reporting();
  if(mouse_subscribe_int(&mouse_bit_no)) return 1;
  if(timer_subscribe_int(&timer_bit_no)) return 1;
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);

  int ipc_status;
  message msg;

  while (count < (idle_time * 60)) {

    int r;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {

        case HARDWARE: {
          if(msg.m_notify.interrupts & timer_irq_set){
            timer_int_handler();
          }
          
          if (msg.m_notify.interrupts & mouse_irq_set) {

            mouse_ih();
            count = 0;
            if (ready) {
              ready = false;
              mouse_print_packet(&pp);
            }
          }

          

          break;
        }

        default: break;
      }
    }
  }
  mouse_unsubscribe_int();
  timer_unsubscribe_int();
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
