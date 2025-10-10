// Este código va al Arduino UNO virtual en Proteus
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_INA219 ina219;

void setup(void) {
  Serial.begin(9600);
  // ... código del sensor INA219 virtual
}

void loop(void) {
  // Medir con sensor INA219 VIRTUAL en Proteus
  float shuntVoltage_mV = ina219.getShuntVoltage_mV();
  float busVoltage_V = ina219.getBusVoltage_V();
  float loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000.0);
  
  float Rshunt = 0.01;
  float current_mA = shuntVoltage_mV / Rshunt;
  float power_mW = loadVoltage_V * (current_mA / 1000.0);

  // Enviar datos por Serial (COMPIM) a ESP32 física
  Serial.print("V:"); Serial.print(loadVoltage_V, 3);
  Serial.print("|I:"); Serial.print(current_mA, 3);
  Serial.print("|P:"); Serial.print(power_mW, 3);
  Serial.println();
  
  delay(1000);
}
