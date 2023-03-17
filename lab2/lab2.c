#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <lcom/lcf.h>


#include <stdbool.h>
#include <stdint.h>

extern int count = 0;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t bruh;
  if (timer_get_conf(timer, &bruh)) return 1;
  return timer_display_conf(timer, bruh, field);
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  int a = timer_set_frequency(timer, freq);

  return a;
}

int(timer_test_int)(uint8_t time) {
  int r;
  uint8_t thing = 0;
  if(timer_subscribe_int(&thing)){
    return 1;
  }

  uint32_t bit = BIT(thing);
  int ipc_status;
  message msg;

  while(count < (time * 60)) { /* You may want to use a different condition */
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & bit) { /* subscribed interrupt */
                    count++;
                    if(count % 60 == 0){
                      timer_print_elapsed_time();
                    }
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }

  return timer_unsubscribe_int();
}
