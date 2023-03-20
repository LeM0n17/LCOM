#include <lcom/lcf.h>

#include "mouse.h"
#include "i8042.h"

int mouse_hook_id = MOUSE_IRQ_LINE;
int count = 0;
bool mouse_ih_error = false, mouse_print_ready = false;
struct packet parsed_packets;

uint8_t packets[3];
bool found = false;


int(mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = mouse_hook_id;
    return sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

int(mouse_get_status)(uint8_t *st){
    return util_sys_inb(MOUSE_ST_REG, st);
}

int(check_status)(uint8_t st){
    return (st & (PAR_ERR | TO_ERR));
}

int(mouse_read_packet)(){
    return util_sys_inb(MOUSE_OUT_BUF, &packets[count]);
}

void(parse_packets)(){
    parsed_packets.bytes[0] = packets[0];
    parsed_packets.bytes[1] = packets[1];
    parsed_packets.bytes[2] = packets[2];

    parsed_packets.rb = packets[0] & MOUSE_RMB_PRESSED;
    parsed_packets.lb = packets[0] & MOUSE_LMB_PRESSED;
    parsed_packets.mb = packets[0] & MOUSE_MMB_PRESSED;

    parsed_packets.x_ov = packets[0] & MOUSE_X_OVL;
    parsed_packets.y_ov = packets[0] & MOUSE_Y_OVL;

    parsed_packets.delta_x = (uint16_t) packets[1];
    parsed_packets.delta_y = (uint16_t) packets[2];

    if(packets[0] & MOUSE_DELTA_X_MSB){
        parsed_packets.delta_x |= EXTEND;
    }

    if(packets[0] & MOUSE_DELTA_Y_MSB){
        parsed_packets.delta_y |= EXTEND;
    }

    return;

}

void(mouse_ih)(){
    uint8_t st = 0;
    if(mouse_get_status(&st)){
        mouse_ih_error = true;
        uint8_t stub;
        util_sys_inb(MOUSE_OUT_BUF, &stub);
        return;
    } 
    if(check_status(st)) {
        mouse_ih_error = true;
        uint8_t stub;
        util_sys_inb(MOUSE_OUT_BUF, &stub);
        return;
    }
    if(mouse_read_packet()){
        mouse_ih_error = true;
        return;
    }

    if(!found){
        ++count;
        if(count > 2){
            parse_packets();
            count = 0;
            mouse_print_ready = true;
            return;
        } else {
            mouse_print_ready = false;
            return;
        }
    } else {
        mouse_print_ready = false;
        if(packets[count] & MOUSE_FIRST_BYTE){
            packets[0] = packets[count];
            found = true;
            return;
        }
    }
}
