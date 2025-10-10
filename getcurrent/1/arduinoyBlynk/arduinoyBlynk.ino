#define BLYNK_TEMPLATE_ID "TMPL2WFu3U0hd"
#define BLYNK_TEMPLATE_NAME "proyecto Diseño"
#define BLYNK_AUTH_TOKEN "nuaC7Hr60mLKrw5_ghpgPBnl-uOSoYWO"

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>
#include <BlynkSimpleStream.h>

// LCD conectado en modo 4 bits: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);
  Blynk.begin(Serial, BLYNK_AUTH_TOKEN);
  Wire.begin();  // SDA = A4, SCL = A5 en Arduino UNO

  lcd.begin(16, 4); // LCD de 20x4
  lcd.setCursor(0, 0);
  lcd.print("Medidor DC");
  delay(2000);
  lcd.clear();

  if (!ina219.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("INA219 no detectado");
    while (1);  // Se detiene si falla el sensor
  }

  lcd.setCursor(0, 0);
  lcd.print("INA219 listo");
  delay(1500);
  lcd.clear();
}

void loop() {
  Blynk.run();  // Ejecuta Blynk en el loop principal

  // === MEDICIONES ===
  float shuntVoltage_mV = ina219.getShuntVoltage_mV();  // Caída en la Rshunt
  float busVoltage_V    = ina219.getBusVoltage_V();     // Voltaje del bus
  float loadVoltage_V   = busVoltage_V + (shuntVoltage_mV / 1000.0);  // Total

  // === PARÁMETROS ===
  float Rshunt     = 0.01;  // Valor actual de tu resistencia shunt
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
