// Definir pines 
#define sensorPin A0       
#define ledRojo 5         
#define ledVerde 7         
#define relePin 9          
#define umbralHumedad 65   

int humedad;       
int humedadPorcentaje; 

void setup() {

  pinMode(sensorPin, INPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(relePin, OUTPUT);
  
  Serial.begin(9600);
}
void loop() {
  humedad = analogRead(sensorPin);
  humedadPorcentaje = map(humedad, 1023, 0, 0, 100);

  Serial.print("Humedad: ");
  Serial.print(humedadPorcentaje);
  Serial.println("%");

  if (humedadPorcentaje < umbralHumedad) {
    //baja
    digitalWrite(ledRojo, HIGH);  
    digitalWrite(ledVerde, LOW);   
    digitalWrite(relePin, HIGH);  
    Serial.println("Bomba ON");
  } else {
    // alta
    digitalWrite(ledRojo, LOW);    
    digitalWrite(ledVerde, HIGH);  
    digitalWrite(relePin, LOW);    
    Serial.println("Bomba OFF");
  }
  delay(2000);
}
