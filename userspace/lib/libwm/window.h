#ifndef LIBWM_WINDOW_H
#define LIBWM_WINDOW_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <stdint.h>


#define LIBWM_DEFAULT_WIDTH 640
#define LIMWM_DEFAULT_HEIGHT 480
#define LIBWM_DEFAULT_X 0
#define LIBWM_DEFAULT_Y 50
#define LIBWM_MAX_WINDOWS 2
#define LIBWM_DEFAULT_WIN_COLOR 0x808080


typedef enum {
    BWT_TERMINAL,
} BUILTIN_WIN_TYPE;


struct Window {
    uint8_t id : 2;
    uint16_t width : 12;
    uint16_t height : 12;
    uint32_t xpos;
    uint32_t ypos;
    BUILTIN_WIN_TYPE type;
    uint8_t active : 1;
};


void libwm_create_window(BUILTIN_WIN_TYPE type);
void libwm_pop_window(void);                // Pops a window (destroys) from top.
void libwm_destroyall(void);
void libwm_write_inside_win(const char* str, uint32_t color, uint16_t x, uint16_t y, uint8_t winid);
struct Window libwm_get_current_window(void);
struct Window libwm_get_win(uint8_t id);
void reset_win(uint8_t id);

#endif
