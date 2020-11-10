/*

  Берем плату Arduino или совместимую, подключаем по USB, прошиваем этим скетчем и
  запускаем скетч. Выполняем указания в Serial Monitor.

  Важно: надо ввести в Serial Monitor значение в милливольтах - 4 цифры, незначащие
  правые дополнить нулями.

*/

/*
  This example don't use library. It demonstrates TroykaPH calibration procedure
*/

float measureVop() {
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
    float Vop = 0;
    for (uint8_t i = 0; i < 100; i++) {
        ADCSRA |= _BV(ADSC);
        while (bit_is_set(ADCSRA, ADSC))
            ;
        uint8_t low = ADCL;
        uint8_t high = ADCH;

        Vop += (float)((high << 8) | low) * 5. / 1024.;
        delay(10);
    }
    return Vop / 100.;
}

float measureZeroLevel() {
    (void)analogRead(A4);
    float Vzero = 0;
    for (uint8_t i = 0; i < 100; i++) {
        Vzero += (float)analogRead(A4) * 5. / 1024.;
        delay(10);
    }
    return Vzero / 100.;
}

float factor;

void setup() {
    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.print("\n\n\nTroyka pH module zero-shift calibration procedure\n\n");
    Serial.print("- Connect TroykaPH module to Arduino board.\n");
    Serial.print("- Shortcut TroykaPH module BNC input to ground.\n");
    Serial.print("- Measure (using good multimeter) Arduino board feed voltage between 5V and GND pins\n");
    Serial.print("- Input this value in millivolts to Serial Monitor input line, press 'Send' after it.\n");
    Serial.print("For example: if measured value is '4.93' volts - punch '4930'\n\n");
}

void loop() {
    if (Serial.available() > 0) {
        float Vmeasured = (float)Serial.parseInt() / 1000;
        if (Vmeasured == 0)
            return;
        Serial.print("Voltage measured by multimeter (V) = ");
        Serial.println(Vmeasured, 3);
        float Vop = measureVop();
        Serial.print("Voltage of internal reference  (V) = ");
        Serial.println(Vop, 3);
        float VccCalculated = Vop * 5. / 1.1;
        Serial.print("Calculated Vcc                 (V) = ");
        Serial.println(VccCalculated, 3);

        factor = (float)(VccCalculated) / (float)(Vmeasured);

        float VccReal = 5. / factor;
        Serial.print("Real Vcc                       (V) = ");
        Serial.println(VccReal, 3);

        float Vzero = measureZeroLevel();
        Serial.print("Calculated zero-shift voltage  (V) = ");
        Serial.println(Vzero, 3);
        float VzeroReal = Vzero / factor;
        Serial.print("Real zero-shift voltage        (V) = ");
        Serial.println(VzeroReal, 3);

        Serial.print("Calibration complete.");
        Serial.print("\n\nCall in your 'setup()' method {your-pH-sensor-name}.begin(");
        Serial.print(factor, 3);
        Serial.print(", ");
        Serial.print(VzeroReal, 3);
        Serial.print("); \n\n");

        Serial.println("Recalibration is needed if you change Arduino board to another.\n\n");
        delay(5000);
    }
}
