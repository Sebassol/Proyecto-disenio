#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>

// LCD conectado en modo 4 bits: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_INA219 ina219;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();  // SDA = A4, SCL = A5 en Arduino UNO

  lcd.begin(16, 4); // LCD de 16x4
  lcd.setCursor(0, 0);
  lcd.print("Medidor DC");
  delay(2000);
  lcd.clear();

  if (!ina219.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("INA219 no detectado");
    while (1);  // Se detiene si falla el sensor
  }

  // ⚠️ NO usamos setCalibration() porque calibramos manualmente
  lcd.setCursor(0, 0);
  lcd.print("INA219 listo");
  delay(1500);
  lcd.clear();
}

void loop(void) {
  // === MEDICIONES ===
  float shuntVoltage_mV = ina219.getShuntVoltage_mV();  // Caída en la Rshunt
  float busVoltage_V    = ina219.getBusVoltage_V();     // Voltaje del bus
  float loadVoltage_V   = busVoltage_V + (shuntVoltage_mV / 1000.0);  // Total

  // === PARÁMETROS ===
  float Rshunt     = 0.01;  // NUEVO: valor actual de tu resistencia
  float current_mA = shuntVoltage_mV / Rshunt;
  float power_mW   = loadVoltage_V * (current_mA / 1000.0);  // P = V * I

  // === DISPLAY LCD ===
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V: "); lcd.print(loadVoltage_V, 2); lcd.print(" V");

  lcd.setCursor(0, 1);
  lcd.print("I: "); lcd.print(current_mA, 3); lcd.print(" mA");

  lcd.setCursor(0, 2);
  lcd.print("P: "); lcd.print(power_mW / 1000.0, 3); lcd.print(" W");

  // === SERIAL MONITOR ===
  Serial.print("Voltaje: "); Serial.print(loadVoltage_V); Serial.print(" V | ");
  Serial.print("Corriente: "); Serial.print(current_mA); Serial.print(" mA | ");
  Serial.print("Potencia: "); Serial.print(power_mW); Serial.println(" mW");

  delay(1000);
}
