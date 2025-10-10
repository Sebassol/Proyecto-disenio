#include <Wire.h>

// ================== CONFIGURACIÓN MCP3421 ==================
const byte MCP3421_ADDRESS = 0x68;
const byte CONFIG_REGISTER = 0b10010100;  // 18-bit, ganancia x2

// ================== PARÁMETROS ==================
const float VBIASTENSION = 1.46233;
const float FACTOR_CALIBRACION_120V = 71.43;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  // Inicializar MCP3421
  if (!inicializarMCP3421()) {
    while(1); // Detener si hay error
  }
  
  Serial.println("SISTEMA_INICIADO:MCP3421_18BIT");
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
  
  delay(1000);  // Enviar cada segundo
}