// 2022 Ian Moffett <ian@kesscoin.com>
#include <stdint.h>
#include <debug/log.h>
#include <drivers/ps2/Keyboard.h>
#include <drivers/video/FrameBuffer.h>
#include <drivers/rtc/rtc.h>
#include <drivers/audio/pcspkr.h>
#include <util/asm.h>
#include <util/string.h>
#include <util/baremetal_style.h>


#define SYSCALL_COUNT 14

static volatile struct SyscallRegs {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
} __attribute__((packed)) *regs;

extern uint64_t* syscall_return;


static void sys_test(void) {
    log("Hello Syscall!\n", S_INFO);
}


// Returns keyboard status in R15.
static void sys_keyboard_status(void) {
    CLI;
    regs->r15 = ps2_keyb_fetch_status();
    STI;
}


// Returns keyboard scancode in R15.
static void sys_keyb_scancode(void) {
    CLI;
    regs->r15 = ps2_keyb_fetch_scancode();
    STI;
}


// Lowers FrameBuffer canvas X pos by 8.
static void sys_lower_canvasx(void) {
    CLI;
    extern canvas_t canvas;

    if (canvas.x > 0)
        canvas.x -= 8;
    
    STI;
}


// Draws square onto framebuffer (when GUI is here, this syscall can be reserved).
// TODO: When adding GUI, allow reserving this syscall to DE/WM.
//  R15: X.
//  R14: Y
//  R13: Width.
//  R12: Height.
//  R11: Color.
static void sys_lfb_draw_sq(void) {
    CLI;
    extern canvas_t canvas;
    const uint32_t MAX_X = 2000;
    const uint32_t MAX_Y = 2000;
    const uint32_t MAX_W = 2000;
    const uint32_t MAX_H = 2000;
    const uint32_t MAX_COLOR = 0xFFFFFF;

    if (regs->r15 > MAX_X || regs->r14 > MAX_Y || regs->r13 > MAX_W || regs->r12 > MAX_H || regs->r11 > MAX_COLOR)
        return;


    draw_square(canvas, regs->r15, regs->r14, regs->r13, regs->r12, regs->r11);
    STI;
}


// Writes a char onto screen while running in baremetal mode.
static void sys_baremetal_writech(void) {
    CLI;
    char terminated_char[2] = {regs->r15, 0x0};
    log("%s", -1, terminated_char);
    STI;
}


// Get canvas X position (returns X pos in R15).
static void sys_get_canvas_x(void) {
    CLI;
    extern canvas_t canvas;
    regs->r15 = canvas.x;
    STI;
}


static void sys_clear_screen(void) {
    CLI;
    extern canvas_t canvas;
    clearScreen(&canvas, BG_COLOR);
    STI;
}


/*
 *  R15: Frequency (hz).
 *
 */
static void sys_spktest(void) {
    CLI;

    pcspkr_play(regs->r15);

    for (int i = 0; i < 99999999; ++i) {
       CLI; 
    }

    pcspkr_stop();

    STI;
}


// Month is returned in R15.
static void sys_get_month(void) {
    CLI;
    static struct RTCDateTime datetime;
    datetime = rtc_read_date();

    regs->r15 = (uint64_t)dec2str(datetime.month);

    STI;
}

// Year is returned in R15.
static void sys_get_year(void) {
    CLI;
    static struct RTCDateTime datetime;
    datetime = rtc_read_date();

    regs->r15 = (uint64_t)dec2str(datetime.year);

    STI;
}


// Day in R15.
static void sys_get_day(void) {
    CLI;
    static struct RTCDateTime datetime;
    datetime = rtc_read_date();

    regs->r15 = (uint64_t)dec2str(datetime.day);

    STI;
}


// Get canvas Y position (returns Y pos in R15).
static void sys_get_canvas_y(void) {
    CLI;
    extern canvas_t canvas;
    regs->r15 = canvas.y;
    STI;
}


// Waits for interrupt.
static void sys_int_wait(void) {
    STI;
    HLT;
}


// An array of syscall functions.
static void(*syscall_table[SYSCALL_COUNT])(void) = {
    sys_test,                               // 0.
    sys_keyboard_status,                    // 1.
    sys_keyb_scancode,                      // 2.
    sys_int_wait,                           // 3.
    sys_baremetal_writech,                  // 4.
    sys_lower_canvasx,                      // 5.
    sys_lfb_draw_sq,                        // 6.
    sys_get_canvas_x,                       // 7.
    sys_get_canvas_y,                       // 8.
    sys_clear_screen,                       // 9.
    sys_get_month,                          // 10.
    sys_get_year,                           // 11.
    sys_get_day,                            // 12.
    sys_spktest,                            // 13.
};



void dispatch_syscall(uint64_t rax, struct SyscallRegs* registers) {
    if (rax >= SYSCALL_COUNT) return;
    regs = registers;
    syscall_table[rax]();
}
