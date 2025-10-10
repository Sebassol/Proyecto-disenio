void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 - Esperando tension...");
}

void loop() {
  if (Serial.available() > 0) {
    String tension = Serial.readStringUntil('\n');
    tension.trim();
    
    // Mostrar solo el valor numérico de la tensión
    Serial.print("Tension: ");
    Serial.print(tension);
    Serial.println(" V");
  }
  delay(500);
}