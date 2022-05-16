#include <drivers/ps2/Controller.h>
#include <arch/io/io.h>

void ps2_send_reset(void) {
    outportb(0x64, 0xFE);
}
