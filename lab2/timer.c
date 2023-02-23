#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer > 2){
    return 1;
  }
  uint32_t rb = (uint32_t)TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer);
  int out_flag = sys_outb(TIMER_CTRL, rb);
  if(out_flag == 1){
    return 1;
  }

  return util_sys_inb(TIMER_CTRL, st);

}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;
  switch(field){
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      enum timer_init init;
      st = st << 2;
      st = st >> 6;
      if(st == 1){
        init.LSB_only = 1;
      } else if (st == 2){
        init.MSB_only = 1;
      } else if (st == 3){
        init.MSB_after_LSB = 1;
      } else {
        init.INVAL_val = 1;
      }
      val.in_mode = init;
      break;
    case tsf_mode:
      st = st << 4;
      st = st >> 5;
      val.count_mode = st;
      break;
    case tsf_base:
      uint8_t mask = 1;
      st = st & mask;
      val.bcd = st;
      break;
    default:
      return 1;

  }
  
  return timer_print_config(timer, field, val);

}
