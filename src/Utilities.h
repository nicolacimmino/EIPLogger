

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <Arduino.h>
#include <time.h>

namespace Utilities
{
    time_t calculateUnixTime(uint8_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t m, uint8_t s, bool dst);
}

#endif
