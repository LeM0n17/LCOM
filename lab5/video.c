#include "video.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <machine/int86.h>

int(change_mode)(uint16_t mode){
  reg86_t r;
  memset(&r, 0, sizeof(r));
  
  r.intno = 0x10;
  r.ah = AH_DEFAULT;
  r.al = MODE_SET;
  r.bx = BIT(14) | mode;

  if (sys_int86(&r) != CALL_SUCCESS) {
  printf("change_mode: sys_int86() failed \n");
  return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
