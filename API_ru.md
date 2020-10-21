# TroykaPH API (RUS)

## `class TroykaPH`

Создайте объект типа `TroykaPH` для управления [модулем изменения pH жидкостей TroykaPH](https://amperka.ru/product/troyka-ph-sensor).

### `TroykaPH(uint8_t pin)`

Создает новый объект TroykaPH.

- `pin`: аналоговый пин к которому подключен модуль.

### `void begin(float correctionMultiplyer = 1.0)`

Инициализирует библиотеку. Вызовите этот метод до начала взаимодействия с TroykaPH. Например в функции `setup()`.

- `correctionMultiplyer`: корректировочный множитель полученный в результате процедуры калибровки. При отсутствии принимается равным `1`.

### `void update(long periodMilliseconds = 1000)`

Производит периодическое чтение показаний датчика и корректировочных коэфициентов. Разместите его вызов в `loop()`.

- `periodMilliseconds`: период опроса датчик.

### `float read() const;`

Возвращает последнее замеренное значение pH.
