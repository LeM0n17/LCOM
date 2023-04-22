// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "video.h"
#include "keyboard.h"
#include "i8042.h"

extern uint8_t data;
extern int count;
extern vbe_mode_info_t vg_info;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("./trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("./output.txt");

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
  if(vg_init(mode) == NULL){
    vg_exit();
    return 1;
  }

  sleep(delay);
  vg_exit();

  return 0;
}

int kbd_loop(){
    int r = 0;
    uint8_t bit = 0;
    if(kbc_subscribe_int(&bit)) return 1;
    uint32_t kanna = BIT(bit);
    
    int ipc_status;
    message msg;
 
    while(data != KBD_ESC) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & kanna) { /* subscribed interrupt */
                        if(kbc_read_out_buffer(&data)) return 1;
                        kbc_ih();
                        if(kbc_print_codes()) return 1;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if(kbc_unsubscribe_int()){
        return 1;
    }
    return 0;

}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

    if(vg_init(mode) == NULL){
        vg_exit();
        return 1;
    }
    if(vg_draw_rectangle(x,y,width,height,color)){
        vg_exit();
        return 1;
    }

    if(kbd_loop()){
        vg_exit();
        return 1;
    }
    vg_exit();
    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(vg_init(mode) == NULL){
    vg_exit();
    return 1;
  }
  uint32_t width = vg_info.XResolution / no_rectangles;
  uint32_t height = vg_info.YResolution / no_rectangles;
  uint32_t color = 0;

  if(vg_info.MemoryModel == VBE_MODE_INDEXED){
    for(uint16_t i = 0; i < no_rectangles;i++){
      for(uint16_t j = 0; j <no_rectangles;j++){
        color = (first + (i * no_rectangles + j) * step) % BIT(vg_info.BitsPerPixel);
        if(vg_draw_rectangle(j*width,i*height,width,height,color)){
          vg_exit();
          return 1;
        }
      }
    }
  } else if(vg_info.MemoryModel == VBE_MODE_DIRECT){
    uint16_t red, green, blue;
    uint32_t red_mask = 0, green_mask = 0, blue_mask = 0;
    for(uint32_t r = vg_info.RedFieldPosition; r < vg_info.RedFieldPosition + vg_info.RedMaskSize;++r){
      red_mask |= BIT(r);
    }
    for(uint32_t b = vg_info.BlueFieldPosition; b < vg_info.BlueFieldPosition + vg_info.BlueMaskSize;++b){
      blue_mask |= BIT(b);
    }
    for(uint32_t g = vg_info.GreenFieldPosition; g < vg_info.GreenFieldPosition + vg_info.GreenMaskSize;++g){
      green_mask |= BIT(g);
    }
    uint16_t rf = ((first & red_mask) >> vg_info.RedFieldPosition);
    uint16_t bf = ((first & blue_mask) >> vg_info.BlueFieldPosition);
    uint16_t gf = ((first & green_mask) >> vg_info.GreenFieldPosition);
    for(uint16_t i = 0; i < no_rectangles; ++i){
      for(uint16_t j = 0; j < no_rectangles; ++j){
        red = (rf + j * step) % BIT(vg_info.RedMaskSize);
        green = (gf + i * step) % BIT(vg_info.GreenMaskSize);
        blue = (bf + (j + i) * step) % BIT(vg_info.BlueMaskSize);
        color = 0 |(red << vg_info.RedFieldPosition) | (green << vg_info.GreenFieldPosition) | (blue << vg_info.BlueFieldPosition);
        if(vg_draw_rectangle(j*width,i*height,width,height,color)){
          vg_exit();
          return 1;
        }
      }
    }
  }

  if(kbd_loop()){
    vg_exit();
    return 1;
  }
  vg_exit();
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(vg_init(0x105) == NULL){
    vg_exit();
    return 1;
  }

  if(vg_draw_xpm(xpm, x, y)){
    vg_exit();
    return 1;
  }

  if(kbd_loop()){
    vg_exit();
    return 1;
  }
  
  vg_exit();  
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  uint8_t frame_period = 60/fr_rate;
  int16_t delta_x = 0, delta_y = 0;
  uint16_t old_x, old_y;
  bool animation_ongoing = true;
  if(speed > 0){
    if(xi == xf){
      delta_y = speed;
    } else {
      delta_x = speed;
    }
  } else {
    frame_period *= -speed;
    if(xi == xf){
      delta_y = 1;
    } else {
      delta_x = 1;
    }
  }
  if(xf < xi){
    delta_x *= -1;
  }

  if(yf < yi){
    delta_y *= -1;
  }

  if(vg_init(0x105) == NULL){
    vg_exit();
    return 1;
  }

  if(vg_draw_xpm(xpm, xi, yi)) {
		vg_exit();
		return 1;
	}

  uint8_t k, t;
  if(timer_subscribe_int(&t)){
    vg_exit();
    return 1;
  }
  if(kbc_subscribe_int(&k)){
    timer_unsubscribe_int();
    vg_exit();
    return 1;
  }

  uint32_t kbd_int = BIT(k), timer_int = BIT(t);


  int r, ipc_status;
  message msg;
  while(data != KBD_ESC) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & kbd_int) { /* subscribed interrupt */
                    if(kbc_read_out_buffer(&data)) return 1;
                    kbc_ih();
                }
                if(msg.m_notify.interrupts & timer_int){
                  timer_int_handler();
                  if(animation_ongoing){
                    if(count >= frame_period){
                      old_x = xi;
                      old_y = yi;
                      xi = xi + delta_x;
                      yi = yi + delta_y;
                      if((xi >= xf && delta_x > 0) || (yi >= yf && delta_y > 0) || (xi <= xf && delta_x < 0) || (yi<=yf && delta_y < 0)){
                        vg_update(xpm, old_x, old_y, xf, yf);
                        animation_ongoing = false;
                      } else {
                        vg_update(xpm, old_x, old_y, xi, yi);
                      }
                      count = 0;
                    }
                  }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  if(kbc_unsubscribe_int()){
    timer_unsubscribe_int();
    vg_exit();
    return 1;
  }

  if(timer_unsubscribe_int()){
    vg_exit();
    return 1;
  }
  vg_exit();
  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

