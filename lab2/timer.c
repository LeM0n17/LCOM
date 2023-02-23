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
  if(timer < 0 || timer > 2) return 1;
  st = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  switch (timer)
  {
  case 0:
    util_sys_inb(TIMER_0, st);
    break;

  case 1:
    util_sys_inb(TIMER_1, st);
    break;

  case 2:
    util_sys_inb(TIMER_2, st);
    break;
  
  default:
    return 1;
    break;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if(timer < 0 || timer > 2) return 1;
  union timer_status_field_val val;
  switch (field)
  {
  case tsf_all:
    val.byte = st;
    break;

  case tsf_initial:
    st>>5;
    uint8_t mask = 0x03;
    st &= mask;
    switch (st)
    {
    case 1:
      val.in_mode = LSB_only;
      break;

    case 2:
      val.in_mode = MSB_only;
      break;

    case 3:
      val.in_mode = MSB_after_LSB;
      break;
    
    default: val.in_mode = INVAL_val;
      break;
    }
    break;

  case tsf_mode:
    val.count_mode = st;
    break;

  case tsf_base:
    val.bcd = st;
    break;
  
  default:
    break;
  }
  timer_print_config(timer, field, val);
}
