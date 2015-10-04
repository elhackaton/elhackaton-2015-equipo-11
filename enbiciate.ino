#include <Wire.h>
#include <LCD.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
 
// Pines a los que están conectados el "Trig" y "Echo" del sensor de ultrasonidos
#define trigPin 4
#define echoPin 3
 
// Pines a los que están conectados los LED
int redPin = 7;
int greenPin = 6;
int bluePin = 5;
 
// Pin al que conectamos el buzzer
#define buzzer 2
 
// SoftwareSerial mySerial(0, 1); // RX, TX
 
// Conexiones de la pantalla LCD al adaptador
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity

// #define BLUETOOTH_SPEED 57600

int sound = 250;
 
void setup() {
	// Inicialimos el Serial, sensor de ultrasonidos, leds y buzzer (zumbador)
	Serial.begin (9600);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
	pinMode(buzzer, OUTPUT);

 
	// Inicializamos la pantalla LCD
	lcd.begin(16,2);
//	lcd.noBacklight();
 
	// Situamos el cursor en la primera casilla de la primera fila y escribimos un texto
	lcd.setCursor(0, 0);
	lcd.print("enbiciate");


//  Serial.println("Starting config");
//  mySerial.begin(BLUETOOTH_SPEED);
//  delay(1000);




}
 
void loop(){
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Recogemos la duración entre los pulsos recibidos por el "echo"
  distance = (duration/2) / 29.1; //Calculamos la distancia al obstáculo según la duración entre los pulsos recibidos

  // Limpiamos la segunda linea de la pantalla LCD
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }

  // Esta serie de ifs sirven para encender unos led y establecer unos tonos para el zumbador según la distancia al obstáculo
  if (distance <= 30 && distance > 20){
    setColor(0, 255, 0); // led en verde intermitente
    delay(10);
    setColor(0, 0, 0);
    delay(10);
    sound = 200;
  }
  if (distance < 20 && distance > 10){
    setColor(255, 0, 255); // led en naranja
    delay(10);
    setColor(0, 0, 0);
    delay(10);
    sound = 250;
  }
  if (distance < 10 && distance > 2){
    setColor(255, 0, 0); // led en rojo
    sound = 300;
  }
  // En caso de que la distancia sea mayor que 1 metro (100 cm) o menor que 2 cm, indicamos que "no hay obstáculos"
  if (distance > 30 || distance <= 2){
    Serial.println("Sin peligro");
    lcd.setCursor(0, 1);
    lcd.print("Sin peligro");
    noTone(buzzer);
    setColor(0, 255, 0); // led en verde
  }else{
    // En caso de que se encuentre en ese rango mostramos la distancia al obstáculo en centímetros y reproducimos el zumbido oportuno
    Serial.print(distance);
    Serial.println(" cm");
    lcd.setCursor(0, 1);
    lcd.print(int(distance));  
    lcd.setCursor(2, 1);
    lcd.print(" cm");
    tone(buzzer, sound);
  }
  delay(200);
}

void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
