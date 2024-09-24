//preguntar al profe por c y microusb
#include <DHT.h>
#include <Adafruit_BMP085_U.h>

#define DHTPIN 4          
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085_Unified bmp; //clase para trabajr con sensor sacada de pag arduino preguntar al profe

#define RED_PIN 25      
#define GREEN_PIN 26    
float temperature, humidity, pressure;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  if (!bmp.begin()) {
    Serial.println("No se encontró el sensor BMP180");
    while (1);
  }
  
  pinMode(RED_PIN, OUTPUT);   
  pinMode(GREEN_PIN, OUTPUT); 
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); 

  sensors_event_t event;
  bmp.getEvent(&event);
  if (event.pressure) {
    pressure = event.pressure / 100; 
  }

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C  | Humedad: ");
  Serial.print(humidity);
  Serial.print(" %   | Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  if (temperature > 27) { 
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  } 
  else if (temperature > 20) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
  } 
  else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
  }
  delay(2000); 
}

