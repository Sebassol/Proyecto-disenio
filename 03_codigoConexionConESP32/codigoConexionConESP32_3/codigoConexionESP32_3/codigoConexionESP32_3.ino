void setup() {
  Serial.begin(9600);
  // Dar tiempo para estabilizar
  delay(1000);
  Serial.println("Lectura LTC1996 en A0:");
}

void loop() {
  // Leer directamente el pin A0
  int valorADC = analogRead(A0);
  
  // Convertir a voltaje
  float tension = (valorADC * 5.0) / 1023.0;
  
  // Mostrar ambos valores para debug
  Serial.print("ADC: ");
  Serial.print(valorADC);
  Serial.print(" -> ");
  Serial.print(tension, 3);
  Serial.println(" V");
  
  delay(1000);
}
