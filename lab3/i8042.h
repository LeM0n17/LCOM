#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define BIT(n) (1<<n)
#define DELAY_US 20000
#define KBD_ESC 0x81
#define KBD_CMD_REG 0x64


#define KBD_IRQ 1

#define KBD_ST_IBF BIT(1)
#define KBD_OUT_BUF 0x60

#define KBD_PAR_ERR BIT(7)
#define KBD_TO_ERR BIT(6)


#define TWO_BYTE 0xE0
#define MAKE 0x80