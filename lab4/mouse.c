#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"

int mouse_hook_id = 12;
struct packet pp;
int i = 0;
bool ready;

int (mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = mouse_hook_id;
    sys_irqsetpolicy(12, IRQ_ENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
    mouse_enable_data_reporting();
    return 0;
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int(mouse_get_status)(uint8_t *st){
    return util_sys_inb(STATUS_REG, st);
}

int(mouse_read_buffer)(uint8_t port, uint8_t *out){
    int timeout = 0;
    uint8_t st;
    while(timeout < 3){
        if(mouse_get_status(&st) != 0){
            return 1;
        }

        if(st & (OBF | AUX) && !(st & (PARITY | TIME_OUT))){
            if(util_sys_inb(port, out) != 0){
                return 1;
            }
        }

        tickdelay(micros_to_ticks(DELAY_US2));
        timeout++;
    }
    return 1;
}

void(mouse_ih)(){
    uint8_t out, st;
    mouse_get_status(&st);

    if(st & OBF && !(st & (PARITY | TIME_OUT))){
        mouse_read_buffer(OUT_BUF, &out);
        pp.bytes[i] = out;
    }

    if(i == 0 && !(out & FIRST_BYTE)){
        return;
    }

    switch (i)
    {
    case 0:{
        pp.y_ov = out & BIT(7);
        pp.x_ov = out & BIT(6);
        pp.mb = out & BIT(2);
        pp.rb = out & BIT(1);
        pp.lb = out & BIT(0);
        break;
    }

    case 1:{
        pp.delta_x = out;
        if (pp.bytes[0] & BIT(4)) pp.delta_x |= 0xFF00;
        break;
    }

    case 2:{
        pp.delta_y = out;
        if (pp.bytes[0] & BIT(5)) pp.delta_y |= 0xFF00;
        break;
    }
    
    default:
    return;
        break;
    }
    i++;

    if(i == 3){
        ready = true;
        i = 0;
    }
}
