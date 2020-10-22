/*
 * This file is a part of TroykaPH library.
 *
 * Product page: https://amperka.ru/product/zelo-folow-line-sensor
 * © Amperka LLC (https://amperka.com, dev@amperka.com)
 * 
 * Author: Yury Botov <by@amperka.com>
 * License: GPLv3, all text here must be included in any redistribution.
 */

#ifndef __TROYKA_PH_H__
#define __TROYKA_PH_H__

#include "Arduino.h"

class TroykaPH {
public:
    TroykaPH(uint8_t pin);
    void begin(float correction = 1.0, float zeroLevel = 2.0);

    void update(long periodMilliseconds = 1000);

    float read() const;

private:
    uint8_t _pin;
    float _lastValue;
    float _correction;
    float _zeroLevel;
    uint32_t _nextMeasureTime;
    long _readVcc();
};

#endif //__TROYKA_PH_H__