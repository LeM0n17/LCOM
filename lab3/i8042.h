#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US    20000

#define BIT(n) (1 << (n))

#define OUT_BUF 0x60
#define KBC_STAT_REG 0x64
#define KBD_IRQ_LINE 1

#define OBF BIT(0)
#define IBF BIT(1)
#define AUX BIT(5)
#define TIMEOUT BIT(6)
#define PARITY BIT(7)

#define KBD_ESC 0x81
#define TWO_BYTE 0xE0

#define MAKE true
#define BREAK false

#endif
