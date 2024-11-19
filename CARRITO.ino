#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración de la pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C y tamaño de la pantalla

// Pines del sensor ultrasónico
const int trigPin = 7; // Pin para TRIG
const int echoPin = 8; // Pin para ECHO

// Definición pines EnA y EnB para el control de la velocidad
int VelocidadMotor1 = 3; 
int VelocidadMotor2 = 5;

// Definición de los pines de control de giro de los motores In1, In2, In3 e In4
int Motor1A = 13; 
int Motor1B = 12;  
int Motor2C = 11; 
int Motor2D = 10; 

// Sensores infrarrojo - izquierdo y derecho
int infraPin  = 2;    
int infraPin1 = 4;

// Variables para la captura de los valores: 0 - fondo claro y 1 - línea negra
int valorInfra = 0;  
int valorInfra1 = 0;  

// Variables del sensor ultrasónico
long duration;
int distance;

// Configuración inicial
void setup() {
  Serial.begin(9600);
  lcd.init();                // Inicializamos la pantalla LCD
  lcd.backlight();           // Encendemos la luz de fondo de la pantalla

  delay(1000);

  // Configuramos pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Establecemos modo de los pines de los sensores infrarrojo
  pinMode(infraPin, INPUT);    
  pinMode(infraPin1, INPUT);

  // Establecemos modo de los pines del control de motores
  pinMode(Motor1A,OUTPUT);
  pinMode(Motor1B,OUTPUT);
  pinMode(Motor2C,OUTPUT);
  pinMode(Motor2D,OUTPUT);
  pinMode(VelocidadMotor1, OUTPUT);
  pinMode(VelocidadMotor2, OUTPUT);

  // Configuramos los dos motores a velocidad 150/255
  analogWrite(VelocidadMotor1, 200); 
  analogWrite(VelocidadMotor2, 200);  

  // Configuramos sentido de giro
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Robot iniciado");
}

// Función para medir distancia con el sensor ultrasónico
int medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leemos el tiempo del pulso en el pin ECHO
  duration = pulseIn(echoPin, HIGH);

  // Convertimos el tiempo en distancia en cm
  distance = duration * 0.034 / 2; 
  return distance;
}

// Ejecución continua
void loop() {
  // Medir distancia con el sensor ultrasónico
  int distanciaObstaculo = medirDistancia();
  Serial.print("Distancia: ");
  Serial.print(distanciaObstaculo);
  Serial.println(" cm");

  // Mostrar distancia en la pantalla LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distanciaObstaculo);
  lcd.print(" cm");

  // Si hay un obstáculo cerca, detener el robot
  if (distanciaObstaculo <= 10) {
    lcd.setCursor(0, 1);
    lcd.print("Obstaculo!");

    // Detener los motores
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor1B, LOW);
    digitalWrite(Motor2C, LOW);
    digitalWrite(Motor2D, LOW);

    delay(1000); // Esperar un segundo
    return; // Saltar la lógica de los sensores infrarrojo
  }

  // Leemos el valor de los infrarrojo: 0 - fondo claro y 1 - línea negra
  valorInfra = digitalRead(infraPin);   
  valorInfra1 = digitalRead(infraPin1);

  // Mostrar el estado en la pantalla LCD
  if (valorInfra == 0 && valorInfra1 == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Ninguno en linea");
  } else if (valorInfra == 0 && valorInfra1 == 1) {
    lcd.setCursor(0, 1);
    lcd.print("Derecho en linea");
  } else if (valorInfra == 1 && valorInfra1 == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Izq en linea");
  } else if (valorInfra == 1 && valorInfra1 == 1) {
    lcd.setCursor(0, 1);
    lcd.print("Ambos en linea");
  }

  // Lógica de control del robot (motores)
  if (valorInfra == 0 && valorInfra1 == 0) {
    digitalWrite(Motor1A, HIGH);
    digitalWrite(Motor2D, HIGH);
    delay(20);                      
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor2D, LOW);
    delay(20);                     
  } else if (valorInfra == 0 && valorInfra1 == 1) {  
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor2D, LOW);
    delay(25);
    digitalWrite(Motor1A, HIGH);
    digitalWrite(Motor2D, LOW);
    delay(20);
  } else if (valorInfra == 1 && valorInfra1 == 0) { 
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor2D, LOW);
    delay(25);
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor2D, HIGH);
    delay(20);
  } else if (valorInfra == 1 && valorInfra1 == 1) { 
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor1B, LOW);
    digitalWrite(Motor2C, LOW);
    digitalWrite(Motor2D, LOW);
  }
}
