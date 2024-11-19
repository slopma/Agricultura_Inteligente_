#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>

// Credenciales Wi-Fi
const char *WIFI_SSID = "TuSSID";
const char *WIFI_PASS = "TuPassword";

// Ubidots
#define TOKEN "TuTokenDeUbidots"
#define DEVICE_LABEL "multi-sensor"
#define VAR_TEMP "temperature"
#define VAR_HUM "humidity"
#define VAR_PRESSURE "pressure"
#define VAR_LED "led_status"
#define VAR_LED_INTENSITY "led_intensity"

// Pines
#define DHTPIN 4        // Pin del DHT11
#define DHTTYPE DHT11   // Tipo de DHT
#define LED_PIN 2       // Pin del LED (PWM)

// Instancias
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Ubidots ubidots(TOKEN);

// Variables
bool ledStatus = false;
int ledIntensity = 0;

void setup() {
  Serial.begin(115200);
  
  // Configuración de Wi-Fi
  Serial.println("Conectando a Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Conectado!");

  // Inicialización de Ubidots
  ubidots.setDebug(true);
  ubidots.setup();
  ubidots.subscribeLastValue(DEVICE_LABEL, VAR_LED);
  ubidots.subscribeLastValue(DEVICE_LABEL, VAR_LED_INTENSITY);

  // Inicialización de sensores
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("No se encontró el sensor BMP180.");
    while (1);
  }

  // Configuración del LED
  pinMode(LED_PIN, OUTPUT);
  ledcAttachPin(LED_PIN, 0);
  ledcSetup(0, 5000, 8); // Canal 0, 5kHz, resolución 8 bits
}

void loop() {
  // Leer datos de los sensores
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure;
  bmp.getPressure(&pressure);

  // Mostrar datos en Serial
  Serial.print("DHT11 - Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C, Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("BMP180 - Presión: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  // Enviar datos a Ubidots
  ubidots.add(VAR_TEMP, temperature);
  ubidots.add(VAR_HUM, humidity);
  ubidots.add(VAR_PRESSURE, pressure / 100); // Conversión a hPa
  ubidots.publish(DEVICE_LABEL);

  // Actualizar LED según comandos de Ubidots
  ubidots.loop(); // Procesar mensajes entrantes
  ledcWrite(0, ledIntensity); // Control de intensidad del LED

  // Esperar antes del próximo envío
  delay(5000);
}

// Callback para controlar el LED
void callback(char *topic, byte *payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic).endsWith(VAR_LED)) {
    ledStatus = message.toInt();
    digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
    Serial.print("LED Status: ");
    Serial.println(ledStatus);
  }

  if (String(topic).endsWith(VAR_LED_INTENSITY)) {
    ledIntensity = constrain(message.toInt(), 0, 255);
    Serial.print("LED Intensity: ");
    Serial.println(ledIntensity);
  }
}
