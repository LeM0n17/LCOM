#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t beesh = (uint16_t) (TIMER_FREQ / freq);

  uint8_t beets = 0;
  if (timer_get_conf(timer, &beets)) return 1;
  beets &= 0x0F; 

  uint8_t lsb = 0, msb = 0;
  if (util_get_LSB(beesh, &lsb) || util_get_MSB(beesh, &msb)) 
    return 1;

  uint8_t master = (timer << 6) | TIMER_LSB_MSB | beets;
  return sys_outb(TIMER_CTRL, master) | sys_outb(TIMER_0 + timer, lsb) | sys_outb(TIMER_0 + timer, msb);
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) return 1;
  
  uint8_t rbcmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL, rbcmd);
  return util_sys_inb(TIMER_PORT(timer), st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val gay;

  switch (field) {
    case tsf_all: {
      gay.byte = st;
      break;
    }

    case (tsf_initial): {
      st <<= 2;
      st >>= 6;
      switch (st) {
        case 1: {
          gay.in_mode = LSB_only;
          break;
        }

        case 2: {
          gay.in_mode = MSB_only;
          break;
        }

        case 3: {
          gay.in_mode = MSB_after_LSB;
          break;
        }
      
        default:{
          gay.in_mode = INVAL_val;
          break;
        }
      } break;
    }

    case(tsf_mode) :{
      gay.count_mode = (st << 4) >> 5;
      break;
    }
    case (tsf_base) :{
      gay.bcd = st & 1;
      break;
    }

  } 
  return timer_print_config(timer, field, gay);
}
