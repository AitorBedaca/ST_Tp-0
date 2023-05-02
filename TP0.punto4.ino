#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include<DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 23

const int BTN1_PIN = 34;
const int BTN2_PIN = 35;

#define LED 25

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int estado = 1;

int estado_boton1 = 1;
int estado_boton2 = 1;

int umbral = 30 ;
float temperatura;
bool btn1_state = LOW;
bool btn2_state = LOW;

// Variables globales para el temporizador de antirebote
volatile unsigned long btn1_time = 0;
volatile unsigned long btn2_time = 0;
const unsigned long DEBOUNCE_TIME = 50;

// Función para leer el estado del botón 1 con antirebote
bool read_btn1() {
  bool btn1_pressed = false;
  if (digitalRead(BTN1_PIN) == HIGH && (millis() - btn1_time) > DEBOUNCE_TIME) {
    btn1_state = !btn1_state;
    btn1_pressed = true;
    btn1_time = millis();
  }
  return btn1_pressed;
}

// Función para leer el estado del botón 2 con antirebote
bool read_btn2() {
  bool btn2_pressed = false;
  if (digitalRead(BTN2_PIN) == HIGH && (millis() - btn2_time) > DEBOUNCE_TIME) {
    btn2_state = !btn2_state;
    btn2_pressed = true;
    btn2_time = millis();
  }
  return btn2_pressed;
}

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
  pinMode(LED, OUTPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

}

void loop() {
  float temperatura = dht.readTemperature();
  int estado_btn1 = digitalRead (BTN1_PIN);
  int estado_btn2 = digitalRead (BTN2_PIN);
  bool btn1_pressed = read_btn1();
  bool btn2_pressed = read_btn2();
  switch (estado) {
    case 1:
      if ((estado_btn1 == 0) && (estado_btn2 == 0)) {
        estado = 3;
      }
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("T actual: ");
      display.print(temperatura);
      display.setCursor(0, 20);
      display.print("T TH: ");
      display.print(umbral);
      display.display();

      break;
    case 2:
      if (estado_btn1 == 0 && estado_btn2 == 0) {
        estado = 4;
      }
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print(umbral);
      display.display();
      switch (estado_boton1) {
        case 1:
          if (estado_btn1 == 0) {
            estado_boton1 = 2;
          }
          break;
        case 2:
          if (estado_btn1 == 0) {

          } else if (estado_btn1 == 1) {
            estado_boton1 = 1;
            umbral++;
          }
          break;
      }
      switch (estado_boton2) {
        case 1:
          if (estado_btn2 == 0) {
            estado_boton2 = 2;
          }
          break;
        case 2:
          if (estado_btn2 == 0) {

          } else if (estado_btn2 == 1) {
            estado_boton2 = 1;
            umbral--;
          }
          break;
      }
      break;
    case 3:
      if (estado_btn1 == 0 && estado_btn2 == 0) {

      } else if (estado_btn1 == 1 && estado_btn2 == 1) {
        estado = 2;
      }
      break;
    case 4:
      if (estado_btn1 == 0 && estado_btn2 == 0) {

      } else if (estado_btn1 == 1 && estado_btn2 == 1) {
        estado = 1;
      }
      break;
  }

  if (temperatura >= umbral) {
    digitalWrite(LED, HIGH);
    //Serial.println("PASA");
  }
  else {
    //Serial.println(" no PASA");
    digitalWrite(LED , LOW);
  }
  Serial.println(estado);
}
