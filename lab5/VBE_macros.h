#ifndef _LCOM_VBE_MACROS_H_
#define _LCOM_VBE_MACROS_H_

#define BASE_ADRESS 0

#define VBE_AH_FUNCTION_INDICATOR 0x4F
#define VBE_AH_CONTROLLER_INFO 0x00
#define VBE_AL_MODE_INFO 0x01
#define VBE_AL_SET_MODE 0x02
#define VBE_BX_SET_MODE BIT(14)

#define VBE_INT_NUMBER 0x10

#define VBE_RET_SUCESS 0x00
#define VBE_RET_FAIL 0x01
#define VBE_NOT_SUPP 0x02
#define VBE_INVALID 0x03


#endif
