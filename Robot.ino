/*
Arduino script for a robot moving forward and avoiding collisions at 20cm range. Sound alert and distance/warings OLED display
*/

//OLED Config
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Motor A
int enA = 9;
int in1 = 8;
int in2 = 7;

//Motor B
int enB = 6;
int in3 = 5;
int in4 = 4;

//Sonar
int soundPin = 12;
int trigPin = 11; 
int echoPin = 10;
float duration, distance;

void setup() {
  //Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(soundPin, OUTPUT);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void avoidObstacle(){
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  
  //Go backwards 1 sec  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  //Go left 0.5 sec  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(500);

  loop();
}

void alertSound(){
  digitalWrite(soundPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(soundPin, LOW);
}

void distanceOLED(void) {
  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println("Stay far..");

  display.setCursor(5, 25);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.print("Distance: ");
  display.print(distance);
  display.print(" cm");
  display.setCursor(55, 40);
  display.cp437(true);
  display.setTextSize(3);
  if(distance < 10){
    alertSound();
    display.write(19);
  } else if (distance >= 10 && distance <= 20){
    alertSound();
    display.write(33);
  } else {
    display.write(3);
  }
  display.cp437(false);
  
  display.display();
}

void loop() {
  //Set motor speed
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  //Go forward  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  //Sonar scan
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 

  //Display on OLED
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2; 
  distanceOLED();
  
  //Avoid collision if too close
  if(distance < 20) {
    alertSound();
    avoidObstacle();
  }
}
