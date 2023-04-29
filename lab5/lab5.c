// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "video.h"
#include "keyboard.h"

extern uint8_t out;
extern vbe_mode_info_t mode_inf;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (change_mode(mode) != 0) return EXIT_FAILURE;
  sleep(delay);
  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  if(map_vm(mode)) return 1;

  if(change_mode(mode)) return 1;

  uint32_t new_color;
  if(normalize_color(color, &new_color)) return 1;

  if(vg_draw_rectangle(x, y, width, height, color)) return 1;

  uint8_t bit_no;
  kbc_subscribe_int(&bit_no);

  uint32_t irq_set = BIT(bit_no);

  int ipc_status;
  message msg;

  while(out != KBD_ESC){
    if(driver_receive(ANY, &msg, &ipc_status) != 0 ) {
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE:		
                 if (msg.m_notify.interrupts & irq_set) {
                    if(kbc_read_out_buffer(&out)) return 1;
                    kbc_ih();
                    if(kbc_print()) return 1;
                 }
                 break;
             default:
                 break;
         }
    }
  }
  if(kbc_unsubscribe_int())return 1;

  vg_exit();
  
  return EXIT_SUCCESS;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(map_vm(mode) != 0) return 1;

  if(change_mode(mode) != 0) return 1;

  int vertical = mode_inf.YResolution / no_rectangles;
  int horizontal = mode_inf.XResolution / no_rectangles;

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {

      uint32_t color;

      if (mode_inf.MemoryModel == 0x06) {
        uint32_t R = Red(j, step, first);
        uint32_t G = Green(i, step, first);
        uint32_t B = Blue(j, i, step, first);
        color = direct_mode(R, G, B);
      } else {
        color = indexed_mode(j, i, step, first, no_rectangles);
      }

      if (vg_draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color)) return 1;
    }
  }

  uint8_t bit_no;
  kbc_subscribe_int(&bit_no);

  uint32_t irq_set = BIT(bit_no);

  int ipc_status;
  message msg;

  while(out != KBD_ESC){
    if(driver_receive(ANY, &msg, &ipc_status) != 0 ) {
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE:		
                 if (msg.m_notify.interrupts & irq_set) {
                    if(kbc_read_out_buffer(&out)) return 1;
                    kbc_ih();
                    if(kbc_print()) return 1;
                 }
                 break;
             default:
                 break;
         }
    }
  }
  if(kbc_unsubscribe_int())return 1;

  vg_exit();

  return EXIT_SUCCESS;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
