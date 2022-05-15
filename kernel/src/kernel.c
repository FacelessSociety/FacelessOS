#include <debug/log.h>
#include <arch/memory/memory.h>
#include <arch/memory/gdt.h>
#include <interrupts/IDT.h>
#include <interrupts/exceptions.h>
#include <drivers/video/FrameBuffer.h>

canvas_t canvas = {
    .x = 0,
    .y = 55,
    .prevX = 0
};


uint8_t gLegacyModeEnabled = 0;


void log(const char* format, STATUS status, ...) {
  va_list ptr;

  size_t args = 0;

  for (size_t i = 0; i < strlen(format); ++i) {
    if (format[i] == '%') {
      switch (format[i + 1]) {
      case 's':
      case 'd':
        ++args;
        i += 2;
        break;
      }
    }
  }

  // A hacky way to get rid of the warning: second parameter of 'va_start' not last named argument
  STATUS s = status;
  status ^= status;
  status += args;
  va_start(ptr, status);

  int color = 0xFFEA00;

  switch (s) {
  case S_WARNING:
    color = 0x800080;
    kwrite(&canvas, "[WARNING] ", color);
    break;
  case S_INFO:
    color = 0xFFEA00;
    kwrite(&canvas, "[INFO] ", color);
    break;
  case S_PANIC:
    color = 0xFF0000;
    kwrite(&canvas, "** KERNEL PANIC **\n", color);
    break;
  case S_CRITICAL:
    color = 0xFF0000;
    kwrite(&canvas, "[CRITICAL] ", color);
    break;
  }

  for (size_t i = 0; i < strlen(format); ++i) {
    if (format[i] == '%') {
      switch (format[i + 1]) {
      case 's':
        {
          const char* arg = va_arg(ptr, const char*);

          for (size_t j = 0; j < strlen(arg); ++j) {
            char terminated[2] = {arg[j], 0x0};
            kwrite(&canvas, terminated, color);
          }

          ++i;

        }

        continue;
      case 'd':
        {
          int arg = va_arg(ptr, int);

          kwrite(&canvas, dec2str(arg), color);
        }
        ++i;
        continue;
      case 'x':
        {
          int arg = va_arg(ptr, int);
          kwrite(&canvas, (char*)hex2str(arg), color);
          ++i;
          continue;
        }
      }
    }

    char terminated[2] = {format[i], 0x0};
    kwrite(&canvas, terminated, color);
  }
}


static void init(void) {
    gdt_load();
    log("GDT loaded.\n", S_INFO);
   
    // Exceptions.
    for (int i = 0x0; i <= 0xE; ++i) {
        // Reserved exceptions or just ones we don't need.
        switch (i) {
            case 0x1:
            case 0x2:
            case 0x3:
            case 0x9:
                continue;
        }

        idt_set_vector(i, exceptions[i], TRAP_GATE_FLAGS);
    }

    // By binded I mean broken up into bits and placed into an IDT Gate Descriptor struct.
    log("Exception handlers binded.\n", S_INFO);
    idt_install();
    log("IDTR loaded with IDT offset.\n", S_INFO);

}


int _start(framebuffer_t* lfb, psf1_font_t* font, meminfo_t meminfo, void* rsdp, uint8_t legacy_mode) {
  (void)meminfo;
  (void)rsdp;
  canvas.font = font;
  canvas.lfb = lfb;
  gLegacyModeEnabled = legacy_mode;

  init();

  while (1) {
    __asm__ __volatile__("hlt");
  }
}
