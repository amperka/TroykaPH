/*

    На самом деле мы калибруем не датчик а напряжение на внутреннем источнике 
опорного напряжения в микросхеме AtMega. Поэтому датчик подключать не обязательно.
Берем плату Arduino или совместимую, подключаем по USB, прошиваем этим скетчем и
запускаем скетч. Выплняем указания в Serial Monitor.

Важно: надо ввести в Serial Monitor значение в миллвольтах - 4 цифры, незначащие
правые дополнить нулями.

*/

/*
* This example don't use library. It demonstrates TroykaPH calibration procedure
*/

long readVcc() {
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

char buffer[5];
uint8_t position;
long multimeterValue;
long adcValue;
float factor;

void setup() {
    position = 0;
    multimeterValue = 0;
    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.print("Troyka pH module zero-shift calibration\n\n");
    Serial.print("Measure (using multimeter) Arduino board feed voltage between "
                 "5V and GND pin,\n");
    Serial.print("and punch this value in millivolts to Serial Monitor input "
                 "line and press 'Send'.\n");
    Serial.print("For example: if measured value is '4.93' volts - punch "
                 "'4930'<Press Enter here>\n\n");
}

void loop() {
    if (Serial.available() && position <= 3) { 
        buffer[position++] = Serial.read();  // reading 4 bytes fom terminal
        else (void) Serial.read(); // if more symbols - ignore 
    }
    if (!Serial.available() && position == 4) {
        buffer[position] = 0;
        for (int i = 0; i < 4; i++) {
            multimeterValue = multimeterValue * 10 + (buffer[i] - '0');
        }
        adcValue = readVcc();
        factor = (float)multimeterValue / (float)adcValue;
        Serial.print("Calibration factor is: ");
        Serial.println(factor);
        Serial.println("Save it, and use in '.begin(factor);' method.");
        Serial.println(
            "Recalibration is needed if you change Arduino board to another.");
    }
}
