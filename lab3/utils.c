#include <lcom/lcf.h>
#include <stdint.h>

int count = 0;
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = (uint8_t) val;

  return F_OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (uint8_t) (val >> 8);

  return F_OK;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t we = *value;

  if (sys_inb(port, &we)) return 1;
  *value = (uint8_t) we;
  count++;
  return F_OK;
}
