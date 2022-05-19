#ifndef LIBWM_WINDOW_H
#define LIBWM_WINDOW_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <stdint.h>


#define LIBWM_DEFAULT_WIDTH 640
#define LIMWM_DEFAULT_HEIGHT 480
#define LIBWM_DEFAULT_X 0
#define LIBWM_DEFAULT_Y 50
#define LIBWM_MAX_WINDOWS 2


struct Window {
    uint8_t id : 2;
    uint16_t width : 12;
    uint16_t height : 12;
    uint32_t xpos;
    uint32_t ypos;
};


void libwm_create_window(void);
void libwm_pop_window(void);

#endif
