/*
 * This file is a part of TroykaPH library.
 *
 * Product page: https://amperka.ru/product/zelo-folow-line-sensor
 * © Amperka LLC (https://amperka.com, dev@amperka.com)
 * 
 * Author: Yury Botov <by@amperka.com>
 * License: GPLv3, all text here must be included in any redistribution.
 */

#include "troykaph.h"

TroykaPH::TroykaPH(uint8_t pin) {
    _pin = pin;
    _lastValue = 7;
    _correction = 1.0;
    _nextMeasureTime = 0;
}

void TroykaPH::begin(float correction, float zeroLevel) {
    _correction = correction;
    _zeroLevel = zeroLevel;
}

static float fmap(float value, float minVal, float maxVal, float minOut, float maxOut) {
    return (value - minVal) / (maxVal - minVal) * (maxOut - minOut) + minOut;
}

void TroykaPH::update(long periodMilliseconds) {
    constexpr float idealVcc = 5.0;
    constexpr float minPh = 0.0;
    constexpr float maxPh = 14.0;
    constexpr float phHalfRangeInVolts = 0.82;

    float value = 0;
    if (_nextMeasureTime <= millis()) {
        _nextMeasureTime += periodMilliseconds;
        // read value
        (void)analogRead(_pin);
        delay(75);
        for (uint8_t i = 0; i < 10; i++) {
            value += (float)analogRead(_pin) * 5.0 / 1024.;
        }
        value = value / 10;
        // read real Vcc value
        float realVcc = (float)(_readVcc()) / 1000.;

        float result = value * idealVcc / realVcc;
        result /= _correction; // internal reference source correction

        _lastValue = fmap(result, _zeroLevel - phHalfRangeInVolts, _zeroLevel + phHalfRangeInVolts, minPh, maxPh);
    }
}

float TroykaPH::read() const { return _lastValue; }

long TroykaPH::_readVcc() {
    constexpr float adcResolution = 1024.;
    constexpr float meanReferenceVoltage = 1.1;
    constexpr long toMillivolts = 1000L;

    long result = 0;
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

    delay(75);
    // internal reference reading
    for (uint8_t i = 0; i < 10; i++) {
        ADCSRA |= _BV(ADSC);
        while (bit_is_set(ADCSRA, ADSC))
            ;

        uint8_t low = ADCL;
        uint8_t high = ADCH;

        result += (high << 8) | low;
    }
    result /= 10;

    result = (long)(adcResolution * meanReferenceVoltage) * toMillivolts / result;
    return result;
}
