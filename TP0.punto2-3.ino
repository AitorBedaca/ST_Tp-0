#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include<DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 23

#define LED 25

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

float umbral = 30 ;

DHT dht(DHTPIN, DHTTYPE); 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();

  pinMode(LED, OUTPUT);

}

void loop() {

   float temperatura = dht.readTemperature();
   display.clearDisplay();
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0,0);
     display.print("T actual: ");
     display.print(temperatura);
     display.display();
     Serial.println(temperatura);

     if (temperatura >= umbral){
      digitalWrite(LED,HIGH);
      Serial.println("PASA");
      }
      else{
        Serial.println(" no PASA");
        digitalWrite(LED ,LOW);
        }
