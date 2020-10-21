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

class TroykaPH {
public:
    TroykaPH(uint8_t pin);
    void begin(float correctionMultiplyer = 1.0);

    void update(long periodMilliseconds = 1000);

    float read() const;

private:
    uint8_t _pin;
    float _lastValue;
    float _correctionMultiplyer;
    long _nextMeasureTime;
    long _readVcc();
};

#endif //__TROYKA_PH_H__