#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

int hook_id = 1;
uint8_t data = 0;

void(kbc_ih)(){
    uint8_t st = 0;
    kbc_get_status(&st);
    if((st & BIT(0)) && check_status(st) == 0){
        kbc_read_out_buffer(&data);
        break;
    }
}


int(kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}


int(kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}

int(kbc_get_status)(uint8_t *st){
    return util_sys_inb(KBD_CMD_REG, st);
}

int(kbc_read_out_buffer)(uint8_t *data){
    return util_sys_inb(KBD_OUT_BUF, data);
}

int(check_status)(uint8_t st){
    return (st & (KBD_PAR_ERR | KBD_TO_ERR | BIT(5)));
}