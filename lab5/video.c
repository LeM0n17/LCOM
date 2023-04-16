#include <lcom/lcf.h>
#include "video.h"

vbe_mode_info_t vg_info;
void* buffer_base = NULL;

void privctl(phys_bytes adress, size_t size){
    int r;
    struct minix_mem_range mr;

    mr.mr_base = adress;
    mr.mr_limit = adress + size;

    if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
}

int(vg_set_mode)(uint16_t mode){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.intno = VBE_INT_NUMBER;
    r86.ah = VBE_AH_FUNCTION_INDICATOR;
    r86.al = VBE_AL_SET_MODE;
    r86.bx = VBE_BX_SET_MODE | mode;

    if(sys_int86(&r86) != OK){
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }

    if(r86.ah != VBE_RET_SUCESS || r86.al != VBE_AH_FUNCTION_INDICATOR){
        printf("vg_init: function return value different from sucess %x\n", r86.al);
        return 1;
    }

    return 0;
}

void*(vg_init)(uint16_t mode){
    if(vbe_get_mode_info(mode,&vg_info)){
        return NULL;
    }

    if((vg_info.ModeAttributes & BIT(0)) == 0){
        return NULL;
    }

    size_t size = vg_info.XResolution * vg_info.YResolution * vg_info.BitsPerPixel;
    privctl((phys_bytes) vg_info.PhysBasePtr, size);
    buffer_base = vm_map_phys(SELF, (void *) vg_info.PhysBasePtr, size);
    if(buffer_base == MAP_FAILED)
        printf("Mapping failed\n");

    if(vg_set_mode(mode)){
        return NULL;
    }
    return buffer_base;
}
