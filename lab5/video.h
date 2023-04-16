#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include "VBE_macros.h"
#include <lcom/lcf.h>

void* (vg_init)(uint16_t mode);
int (vg_set_mode)(uint16_t mode);
void privctl(phys_bytes adress, size_t size);

#endif
