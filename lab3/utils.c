#include <lcom/lcf.h>

#include <stdint.h>

int count_io = 0;

int (util_sys_inb)(int port, uint8_t *value){
  uint32_t val = *value;
  if(sys_inb(port, &val)) return 1;
  count_io++;
  *value = val;
  return 0;
}
