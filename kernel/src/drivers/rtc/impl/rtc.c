#include <drivers/rtc/rtc.h>
#include <arch/io/io.h>

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

// 2022 Ian Moffett <ian@kesscoin.com>

/*
 *  You read CMOS registers by writing the register
 *  to CMOS_ADDR and than reading CMOS_DATA.
 */


// Checks if RTC is updating.
static uint8_t rtc_updating(void) {
    outportb(CMOS_ADDR, 0x0A);               // Select status register A.
    uint32_t status = inportb(CMOS_DATA);   // Read status register A.
    return status & 0x80;                   // Bit 0x80 is set then it is updating.
}


static uint8_t rtc_read_reg(uint32_t reg) {
    outportb(CMOS_ADDR, reg);
    return inportb(CMOS_DATA);
}


struct RTCDateTime rtc_read_date(void) {
    while (rtc_updating());                 // Spin until RTC is not updating.
    
    struct RTCDateTime date_time;
    date_time.second  = rtc_read_reg(0x00); // Read seconds.                                        //
    date_time.minute = rtc_read_reg(0x02);  // Read minutes.
    date_time.hour = rtc_read_reg(0x04);    // Read hours.
    date_time.day = rtc_read_reg(0x07);     // Read day.
    date_time.month = rtc_read_reg(0x08);   // Read month.
    date_time.year = rtc_read_reg(0x09);    // Read year.

    uint8_t regB = rtc_read_reg(0x0B);
    
    // Convert BCD to binary values if we need to.
    if (!(regB & 0x04)) {
        date_time.second = (date_time.second & 0x0F) + ((date_time.second / 16) * 10);
        date_time.minute = (date_time.minute & 0x0F) + ((date_time.minute / 16) * 10);
        date_time.hour = ((date_time.hour & 0x0F) + (((date_time.hour & 0x70) / 16) * 10) ) | (date_time.hour & 0x80);
        date_time.day = (date_time.day & 0x0F) + ((date_time.day / 16) * 10);
        date_time.month = (date_time.month & 0x0F) + ((date_time.month / 16) * 10);
        date_time.year = (date_time.year & 0x0F) + ((date_time.year / 16) * 10);
    }

    return date_time;
}
