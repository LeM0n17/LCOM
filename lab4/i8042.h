#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MOUSE_IRQ_LINE 12

#define MOUSE_ST_REG 0x64
#define MOUSE_CTRL_REG 0x64
#define MOUSE_IN_BUF 0x60
#define MOUSE_OUT_BUF 0x60
#define KBC_WRITE_MOUSE_CMD 0xD4

#define MOUSE_ACK 0xFA
#define MOUSE_ACK_ER 0xFE

#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

#define MOUSE_FIRST_BYTE BIT(3)



#endif