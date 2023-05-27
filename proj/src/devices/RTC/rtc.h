#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>

#define RTC_REG_UPDATING 10
#define RTC_REG_COUNTING 11
#define RTC_REG_INPUT 0x70
#define RTC_REG_OUTPUT 0x71

#define RTC_BIN_MASK BIT(2)
#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4   
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9 

int getRTCOutput(uint8_t cmd, uint8_t *output);
bool isBinary();
uint8_t toBinary(uint8_t num);
int readRTC(uint8_t what, uint8_t* result);

#endif
