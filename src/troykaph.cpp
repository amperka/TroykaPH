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
    _correctionMultiplyer = 1.0;
    _nextMeasureTime = 0;
}

void TroykaPH::begin(float correctionMultiplyer = 1.0) {
    _correctionMultiplyer = correctionMultiplyer;
}

void TroykaPH::update(long periodMilliseconds = 1000) {
    if (_nextMeasureTime <= millis()) {
        _nextMeasureTime += periodMilliseconds;

        (void)analogRead(_pin);
        delay(75);
        float value = analogRead(_pin) * 5.0 / 1024.;

        _lastValue = value * (5000L / _readVcc()) * _correctionMultiplyer;
    }
}

float TroykaPH::read() const { return _lastValue; }

long TroykaPH::_readVcc() {
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
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC))
        ;

    uint8_t low = ADCL;
    uint8_t high = ADCH;

    long result = (high << 8) | low;

    result = 1125300L / result;
    return result;
}