#include "MS5611.h"

MS5611 baro;
int32_t pressure;

void setup() {
  // Start barometer
  baro = MS5611();
  baro.begin();
  // Start serial (UART)
  Serial.begin(9600);
  delay(2);
}

void loop() {
  // Read pressure
  pressure = baro.getPressure();
  // Send pressure via serial (UART);
  Serial.println(pressure);
}
