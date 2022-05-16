#ifndef RTC_H
#define RTC_H

#include <stdint.h>


// 2022 Ian Moffett <ian@kesscoin.com>


struct RTCDateTime {
    int64_t hour;
    int64_t minute;
    int64_t second;
    int64_t year;
    int64_t month;
    int64_t day;
};


struct RTCDateTime rtc_read_date(void);


#endif
