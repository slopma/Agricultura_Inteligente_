#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define sensorHumedadPin A0
#define sensorNivelAguaPin A1
#define ledRojo 5
#define ledVerde 7
#define umbralHumedad 55
#define umbralNivelAgua 50

int humedad;
int nivelAgua;
int humedadPorcentaje;
int nivelAguaPorcentaje;

// Crear objeto LCD en la dirección 0x27 con 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(sensorHumedadPin, INPUT);
  pinMode(sensorNivelAguaPin, INPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  Serial.begin(9600);
  
  // Inicializar LCD con el número de columnas y filas
  lcd.begin(16, 2);
  lcd.backlight(); // Encender la luz de fondo del LCD
}

void loop() {
  // Lectura de sensores
  humedad = analogRead(sensorHumedadPin);
  nivelAgua = analogRead(sensorNivelAguaPin);
  humedadPorcentaje = map(humedad, 1023, 0, 0, 100);
  nivelAguaPorcentaje = map(nivelAgua, 1023, 0, 0, 100);

  // Mostrar resultados en el monitor serial
  Serial.print("Humedad: ");
  Serial.print(humedadPorcentaje);
  Serial.println("%");
  
  Serial.print("Nivel de Agua: ");
  Serial.print(nivelAguaPorcentaje);
  Serial.println("%");

  // Control del LED según la humedad
  if (humedadPorcentaje < umbralHumedad) {
    digitalWrite(ledRojo, HIGH);  // Enciende rojo
    digitalWrite(ledVerde, LOW); // Apaga verde
  } else {
    digitalWrite(ledRojo, LOW);  // Apaga rojo
    digitalWrite(ledVerde, HIGH); // Enciende verde
  }

  // Mostrar en pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humedadPorcentaje);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Nivel Agua: ");
  lcd.print(nivelAguaPorcentaje);
  lcd.print("%");

  delay(2000);
}
