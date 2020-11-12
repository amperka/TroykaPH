# TroykaPH API

## `class TroykaPH`

Create an object of type `TroykaPH` to control the [module for measuring pH of liquids TroykaPH] (https://amperka.ru/product/troyka-ph-sensor).

### `TroykaPH(uint8_t pin)`

Creates a new TroykaPH object.

- `pin`: analog pin to which the module is connected.

### `void begin(float correction = 1.0, float zeroShift = 2.0)`

- `correction` - AVR's internal voltage reference offset correction.
- `zeroShift` - correction of the real shift `0` during measurements.

Initializes the library. Call this method before you begin interacting with TroykaPH. For example, in the `setup()`.

- `correctionMultiplier`: correction factor obtained as a result of the calibration procedure. If absent, it is taken equal to `1`.

### `void update(long periodMilliseconds = 1000)`

Performs periodic reading of sensor and correction factors. Place his call in `loop()`.

- `periodMilliseconds`: sensor polling period.

### `float read() const;`

Returns the last measured pH value.
