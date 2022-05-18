#ifndef PCI_H
#define PCI_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <stdint.h>


uint8_t pci_read_classid(uint8_t bus, uint8_t slot, uint8_t func);
uint8_t pci_read_subclass_id(uint8_t bus, uint8_t slot, uint8_t func);
uint16_t pci_read_vendor_id(uint8_t bus, uint8_t slot, uint8_t func);
uint8_t pci_vendor_id_valid(uint16_t id);
void pci_bruteforce_dump(void);


typedef enum {
    NOT_FINALIZED,
    MASS_STORAGE_CONTROLLER,
    NETWORK_CONTROLLER,
    DISPLAY_CONTROLLER,
    MULTI_MEDIA_DEVICE,
    MEMORY_CONTROLLER,
    BRIDGE_DEVICE,
    SCC,
    BASE_SYSTEM_PERIPHERALS,
    INPUT_DEVICES,
    DOCKING_STATIONS,
    PROCESSORS,
    SERIAL_BUS_CONTROLLERS,
    WIRELESS_CONTROLLER,
    INTELLIGENT_IO_CONTROLLER,
    SATELLITE_COMMUNICATION_CONTROLLERS,
    DASP_CONTROLLER,
    PROCESSING_ACCESLERATORS,
    NE,
    RESERVED
} BASE_CLASS;


#endif
