#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// ================== CONFIGURACIÓN MCP3421 ==================
const byte MCP3421_ADDRESS = 0x68;
const byte CONFIG_REGISTER = 0b10010100;  // 18-bit, ganancia x2

// ================== PARÁMETROS ==================
const float VBIASTENSION = 1.46233;
const float FACTOR_CALIBRACION_120V = 71.43;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  lcd.begin(16, 4);
  lcd.setCursor(0, 0);
  lcd.print("Medidor 120V AC");
  lcd.setCursor(0, 1);
  lcd.print("MCP3421 18-bit");
  delay(2000);

  if (!inicializarMCP3421()) {
    lcd.clear();
    lcd.print("ERROR MCP3421");
    while(1);
  }
  
  lcd.clear();
  lcd.print("Sistema Listo");
  Serial.println("INICIADO");
}

bool inicializarMCP3421() {
  Wire.beginTransmission(MCP3421_ADDRESS);
  Wire.write(CONFIG_REGISTER);
  return (Wire.endTransmission() == 0);
}

long leerMCP3421() {
  byte data[3];
  int intentos = 0;
  
  while (intentos < 5) {
    Wire.requestFrom(MCP3421_ADDRESS, 3);
    
    if (Wire.available() == 3) {
      data[0] = Wire.read();
      data[1] = Wire.read();
      data[2] = Wire.read();
      
      if ((data[2] & 0x80) == 0) {
        long valor = ((long)(data[0] & 0x03) << 16) | ((long)data[1] << 8) | (data[2] & 0xFC);
        if (valor & 0x20000) valor |= 0xFFFC0000;
        return valor;
      }
    }
    intentos++;
    delay(15);
  }
  return 0;
}

float leerVoltajePreciso() {
  long valorADC = leerMCP3421();
  return (valorADC * 1.024) / 131072.0;
}

void loop(void) {
  // Leer voltaje del MCP3421
  float voltajeConBias = leerVoltajePreciso();
  
  // Compensar bias para obtener señal AC pura
  float voltajeAC = voltajeConBias - (VBIASTENSION / 2.0);
  
  // Reconstruir 120V RMS real
  float voltaje120Vrms = (abs(voltajeAC) / 1.414) * FACTOR_CALIBRACION_120V * 2.0;
  
  // ENVIAR SOLO EL VALOR DE TENSIÓN A ESP32
  Serial.println(voltaje120Vrms, 2);  // Solo el número con 2 decimales
  
  // Mostrar en LCD virtual (opcional)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tension AC:");
  lcd.setCursor(0, 1);
  lcd.print(voltaje120Vrms, 1);
  lcd.print(" V RMS");
  lcd.setCursor(0, 2);
  lcd.print("ADC:");
  lcd.print(voltajeConBias, 3);
  lcd.print("V");
  
  delay(1000);  // Enviar cada segundo
}