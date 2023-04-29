#include "video.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <machine/int86.h>

vbe_mode_info_t mode_inf;
uint8_t *video_mem;

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

int map_vm(uint16_t mode){

  memset(&mode_inf, 0, sizeof(mode_inf));
  if(vbe_get_mode_info(mode, &mode_inf)) return 1;

  struct minix_mem_range mr;  /* VRAM's physical addresss */
  unsigned int vram_size;

  unsigned int bytes_per_pixel = (mode_inf.BitsPerPixel + 7) / 8;
  vram_size = mode_inf.XResolution * mode_inf.YResolution * bytes_per_pixel;

  /* Allow memory mapping */

  mr.mr_base = mode_inf.PhysBasePtr;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if (video_mem == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

int (normalize_color)(uint32_t color, uint32_t *new_color) {
  if (mode_inf.BitsPerPixel == 32) {
    *new_color = color;
  } else {
    *new_color = color & (BIT(mode_inf.BitsPerPixel) - 1);
  }
  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  
  if(x > mode_inf.XResolution || y > mode_inf.YResolution) return 1;

  unsigned int BytesPerPixel = (mode_inf.BitsPerPixel + 7) / 8;

  unsigned int index = (mode_inf.XResolution * y + x) * BytesPerPixel;

  if(memcpy(&video_mem[index], &color, BytesPerPixel) == NULL) return 1;

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color){
  for(unsigned i = 0; i < width; i++){
    if(draw_pixel(x + i, y, color) != 0) return 1;
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(int i = 0; i < height; i++){
    if(vg_draw_hline(x, y + i, width, color)){
      vg_exit();
      return 1;
    }
  }
  return 0;
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_inf.RedFieldPosition) | (G << mode_inf.GreenFieldPosition) | (B << mode_inf.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << mode_inf.BitsPerPixel);
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << mode_inf.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << mode_inf.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << mode_inf.BlueMaskSize);
}

uint32_t (R)(uint32_t first){
  return ((1 << mode_inf.RedMaskSize) - 1) & (first >> mode_inf.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_inf.GreenMaskSize) - 1) & (first >> mode_inf.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_inf.BlueMaskSize) - 1) & (first >> mode_inf.BlueFieldPosition);
}
