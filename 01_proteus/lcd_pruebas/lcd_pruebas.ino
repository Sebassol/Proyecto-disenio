#include <LiquidCrystal.h> // incluimos la librería del display
LiquidCrystal lcd(2,3,4,5,6,7); // Conexiones(rs, enable, d4, d5, d6, d7)

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2); // iniciamos la interfaz del lcd
  lcd.clear(); // Limpiamos la pantalla
  lcd.setCursor(0,0); // colocamos el cursor en la columna 0 fila 0
  lcd.print("Prueba"); // imprimimos texto
  lcd.setCursor(0,1); // Colocamos el cursor en la columna 0 fila 1
  lcd.print("LCD probando");
}

void loop() {
}

