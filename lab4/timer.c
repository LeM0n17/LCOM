#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#define TIMER_SEL(timer) (timer << 6)

int hook_id = 0;
int timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t f = (uint16_t)(TIMER_FREQ / freq);
  uint8_t status = 0;
  if(timer_get_conf(timer, &status)){
    return 1;
  }
  status &= 0x0F;
  uint8_t cw = TIMER_SEL(timer) | status;
  cw |= TIMER_LSB_MSB;
  if(sys_outb(TIMER_CTRL, cw)){
    return 1;
  }
  uint8_t lsb = 0;
  uint8_t msb = 0;
  if(util_get_LSB(f, &lsb)  || util_get_MSB(f,&msb)){
    return 1;
  }
  if(sys_outb(0x40 + timer, lsb)){
    return 1;
  }
  if(sys_outb(0x40 + timer, msb)){
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  ++timer_counter;
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
      uint8_t mask = ~BIT(2);
      st = st << 4;
      st = st >> 5;
      if(st == 6 || st == 7){
        st &= mask;
      }
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
