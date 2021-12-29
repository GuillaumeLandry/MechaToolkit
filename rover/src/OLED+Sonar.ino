
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int soundPin = 8;
const int trigPin = 9; 
const int echoPin = 10;
float duration, distance; 

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(soundPin, OUTPUT); 
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void loop() {
   digitalWrite(trigPin, LOW); 
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH); 
   delayMicroseconds(10); 
   digitalWrite(trigPin, LOW); 

   duration = pulseIn(echoPin, HIGH); 
   distance = (duration*.0343)/2; 
   distanceLabel();

   if(distance < 90) {
       digitalWrite(soundPin, HIGH);
       delayMicroseconds(100);
       digitalWrite(soundPin, LOW);
   }

   Serial.print("Distance: "); 
   Serial.println(distance); 
   delay(100);
}

void distanceLabel(void) {
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
    display.write(19);
  } else if (distance >= 10 && distance <= 20){
    display.write(33);
  } else {
    display.write(3);
  }
  display.cp437(false);
  
  display.display();
}
