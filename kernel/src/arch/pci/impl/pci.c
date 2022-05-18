#include <arch/pci/pci.h>
#include <drivers/video/FrameBuffer.h>
#include <util/baremetal_style.h>
#include <arch/io/io.h>
#include <debug/log.h>


#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

static const char* const BASE_CLASS_TYPES[] = {
    "NOT_FINALIZED",
    "MASS_STORAGE_CONTROLLER",
    "NETWORK_CONTROLLER",
    "DISPLAY_CONTROLLER",
    "MULTI_MEDIA_DEVICE",
    "MEMORY_CONTROLLER",
    "BRIDGE_DEVICE",
    "SCC",                              // Simple communication controllers.
    "BASE_SYSTEM_PERIPHERALS",
    "INPUT_DEVICES",
    "DOCKING_STATIONS",
    "PROCESSORS",
    "SERIAL_BUS_CONTROLLERS",
    "WIRELESS_CONTROLLER",
    "INTELLIGENT_IO_CONTROLLER",
    "SATELLITE_COMMUNICATION_CONTROLLERS",
    "ENCRYPTION/DECRYPTION CONTROLLERS",
    "DASP_CONTROLLER",
    "PROCESSING_ACCELERATORS",
    "NE",
    "RESERVED",
    "INVALID"
};


static uint16_t read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    uint32_t address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    outportl(CONFIG_ADDRESS, address);
    tmp = (uint16_t)((inportl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}


uint8_t pci_read_classid(uint8_t bus, uint8_t slot, uint8_t func) {
    return read_word(bus, slot, func, 0xA) >> 8;
}


uint8_t pci_read_subclass_id(uint8_t bus, uint8_t slot, uint8_t func) {
    return read_word(bus, slot, func, 0xA) & 0xFF;
}


uint16_t pci_read_vendor_id(uint8_t bus, uint8_t slot, uint8_t func) {
    return read_word(bus, slot, func, 0x0);
}


uint8_t pci_vendor_id_valid(uint16_t id) {
    return id != 0xFFFF;
}


void pci_bruteforce_dump(void) {
    extern canvas_t canvas;
    uint16_t lines = 0;

    clearScreen(&canvas, BG_COLOR);

    for (uint16_t bus = 0; bus < 256; ++bus) {
        for (uint8_t slot = 0; slot < 32; ++slot) {
            for (uint8_t func = 0; func < 8; ++func) {
                if (pci_vendor_id_valid(pci_read_vendor_id(bus, slot, func))) {
                    const char* baseclass_type;

                    if (pci_read_classid(bus, slot, func) > 0x13) 
                        baseclass_type = "RESERVED";
                    else
                        baseclass_type = BASE_CLASS_TYPES[pci_read_classid(bus, slot, func)];

                    log("Device found on bus %d, slot %d with type %s.\n", S_INFO, bus, slot, baseclass_type);
                    ++lines;
                }

                if (lines > 30) {
                    clearScreen(&canvas, BG_COLOR);
                    lines = 0;
                }
            }
        }
    }

}
