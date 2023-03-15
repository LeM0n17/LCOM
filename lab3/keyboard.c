#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

int kbd_hook_id = 1;
uint8_t data = 0;
uint8_t scan_codes[2];
int print = 0;
int num = 1;

void(kbc_ih)(){
    if(data == TWO_BYTE){
        print = 0;
        num = 2;
        scan_codes[0] = data;
    } else {
        print = 1; 
        if(scan_codes[0] == TWO_BYTE){
            scan_codes[1] = data;
        } else {
            num = 1;
            scan_codes[0] = data;

        }
    }
}


int(kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = kbd_hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}


int(kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&kbd_hook_id);
}

int(kbc_get_status)(uint8_t *st){
    return util_sys_inb(KBD_CMD_REG, st);
}

int(kbc_read_out_buffer)(uint8_t *data){
    uint8_t st = 0;
    int err = 5;
    while(err != 0){
        if(kbc_get_status(&st)){
            return 1;
        }
        if((st & BIT(0)) && check_status(st) == 0){
            return util_sys_inb(KBD_OUT_BUF, data);
            
        }
        err--;
        if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
    }
    return 1;
}

int(check_status)(uint8_t st){
    return (st & (KBD_PAR_ERR | KBD_TO_ERR | BIT(5)));
}

int(kbc_print_codes)(){
    if(print){
        if(data < MAKE){
            return kbd_print_scancode(true, num, scan_codes);
        }
        return kbd_print_scancode(false, num, scan_codes);
    }
    return 0;
}


int(kbc_send_cmd)(uint8_t port, uint8_t cmd){
    int i = 5;
    uint8_t st = 0;
    while(i != 0){
        if(kbc_get_status(&st)){
            return 1;
        }
        if(st & KBD_ST_IBF){
            i--;
            if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
            continue;
        }
        break;
        
    }
    if(i == 0) return 1; //Timeout

    return sys_outb(port, cmd);
}

int(kbc_reenable_int)(){
    if(kbc_send_cmd(IN_BUF, READ_CMD_BYTE)) return 1;

    uint8_t c;
    if(kbc_read_out_buffer(&c)) return 1;

    c |= KBD_REENABLE_INT;

    if(kbc_send_cmd(IN_BUF, WRITE_CMD_BYTE)) return 1;

    return (kbc_send_cmd(IN_BUF_ARGS,c));

}
