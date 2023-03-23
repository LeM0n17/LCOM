// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "mouse.h"

extern bool mouse_print_ready, mouse_ih_error;
extern struct packet parsed_packets;
int timer_counter = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
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


int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no = 0;
    if(mouse_enable_data_reporting()){
      return 1;
    }
    if(mouse_subscribe_int(&bit_no)){
      return 1;
    }
    uint32_t bit = BIT(bit_no);
    int ipc_status,r;
    message msg;
    while(cnt){
      if((r = driver_receive(ANY, &msg,&ipc_status)) != 0){
        printf("driver_receive failed with: %d", r);
        continue;
      }
      if(is_ipc_notify(ipc_status)){
        switch(_ENDPOINT_P(msg.m_source)){
          case HARDWARE:{
            if(msg.m_notify.interrupts & bit){
              mouse_ih();
              if(mouse_ih_error){
                return 1;
              }
              if(mouse_print_ready){
                cnt--;
                mouse_print_packet(&parsed_packets);
              }
            }
            break;
          }
          default:
            break;
        }
      }
    }
    
    return mouse_unsubscribe_int();
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t bit = 0;
  uint8_t time = 0;
  if(mouse_enable_data_reporting()){
    return 1;
  }
  if(mouse_subscribe_int(&bit)){
    return 1;
  }
  if(timer_subscribe_int(&time)){
    return 1;
  }

  uint32_t timer_int = BIT(time);
  uint32_t mouse_int = BIT(bit);
  uint8_t idle = idle_time;

  int ipc_status, r;
  message msg;
  while(idle){
    if((r = driver_receive(ANY, &msg, &ipc_status))!= 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    
      if(is_ipc_notify(ipc_status)){
        switch(_ENDPOINT_P(msg.m_source)){
          case HARDWARE:{
            if(msg.m_notify.interrupts & timer_int){
              timer_int_handler();
              if(timer_counter == 60){
                timer_counter = 0;
                idle--;
              }
            }
          if(msg.m_notify.interrupts & mouse_int){
            mouse_ih();
            if(mouse_ih_error){
              return 1;
            }
            if(mouse_print_ready){
              idle = idle_time;
              timer_counter = 0;
              mouse_print_packet(&parsed_packets);
            }
          }
          break;
        }
        default:
          break;
      }
    }
  }
  if(timer_unsubscribe_int()){
    return 1;
  }
  return mouse_unsubscribe_int();
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

