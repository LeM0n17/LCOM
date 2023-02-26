#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t f = (uint16_t) (TIMER_FREQ / freq);
  uint8_t cw = 0;
  switch(timer){
    case 0: {
      cw = cw | TIMER_SEL0;
      break;
    }
    case 1: {
      cw = cw | TIMER_SEL1;
      break;
    }
    case 2: {
      cw = cw | TIMER_SEL2;
      break;
    }
    default:
      return 1;
  }
  cw = cw | TIMER_LSB_MSB;
  uint8_t status;
  if(timer_get_conf(timer, &status)){
    return 1;
  }
  uint8_t mask = BIT(3)|BIT(2)|BIT(1)|BIT(0);
  cw  = cw | (status & mask);
  if(sys_outb(TIMER_CTRL, cw)){
    return 1;
  }
  uint8_t lsb;
  uint8_t msb;
  if(util_get_LSB(f, &lsb)  || util_get_MSB(f,&msb)){
    return 1;
  }
  if(sys_outb(0x40 + timer, msb)){
    return 1;
  }
  if(sys_outb(0x40 + timer, lsb)){
    return 1;
  }
  return 0;
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

  return util_sys_inb(0x40 + timer, st);

}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;
  switch(field){
    case tsf_all: {
      val.byte = st;
      break;
    }
    case tsf_initial: {
      enum timer_init init;
      st = st << 2;
      st = st >> 6;
      if(st == 1){
        init = LSB_only;
      } else if (st == 2){
        init = MSB_only;
      } else if (st == 3){
        init = MSB_after_LSB;
      } else {
        init = INVAL_val;
      }
      val.in_mode = init;
      break;
    }
    case tsf_mode: {
      st = st << 4;
      st = st >> 5;
      val.count_mode = st;
      break;
    }
      
    case tsf_base: {
      uint8_t mask = 1;
      st = st & mask;
      val.bcd = st;
      break;
    }
    default:
      return 1;

  }
  
  return timer_print_config(timer, field, val);

}
