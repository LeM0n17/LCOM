#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

int kbc_hook_id = 1;
uint8_t out = 0;
int size = 0;
uint8_t codes[2];
int print = 0;

int (kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = kbc_hook_id;
    return sys_irqsetpolicy(KBD_IRQ_LINE, IRQ_ENABLE | IRQ_EXCLUSIVE, &kbc_hook_id);
}

int (kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&kbc_hook_id);
}

int(kbc_get_status)(uint8_t *st){
    return util_sys_inb(KBC_STAT_REG, st);
}

int(kbc_read_out_buffer)(uint8_t *out){
    int timeout = 5;
    uint8_t st = 0;

    while(timeout != 0){
        if(kbc_get_status(&st))return 1;

        if((st & OBF) && ((st & (PARITY | TIMEOUT | AUX)) == 0)){
            return util_sys_inb(OUT_BUF, out);
        }

        timeout--;
        if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
    }
    return 1;
}

void (kbc_ih)(){
    if(out == TWO_BYTE){
        print = 0;
        codes[0] = out;
        size = 2;
    }else{
        print = 1;
        if(codes[0] == TWO_BYTE){
            codes[1] = out;
        }else{
            codes[0] = out;
            size = 1;
        }
    }
}

int (kbc_print)(){
    if(print){
        if(out < 0x80){
            return kbd_print_scancode(true, size, codes);
        }
        return kbd_print_scancode(false, size, codes);
    }
    return 0;
}
