//Зададим включения библиотек
//При использование <> в include сначала ищет в глобальных библиотеках
//При использованиие "" в include сначала ищет в папке с текущем файлом
#include <Arduino.h>
#include <GyverEncoder.h>
#include <FastLED.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include "dog.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font
#include "Free_Fonts.h"
#define FF18    FreeMonoBoldOblique12pt7b

//в определениях указываем первоначально все используемые пины
//по принципу #define <что меняем>  <на что меняем>
//энкодер KY-040
#define ENC_CLK   17
#define ENC_DT    16
#define ENC_SW    5

//дисплей ST7789 240*240 SPI переопределил в файле User_Setup.h в библиотеке TFT_eSPI
//#define TFT_DC    2
//#define TFT_RST   4 
//#define TFT_MOSI  13   // for hardware SPI data pin (all of available pins)
//#define TFT_SCLK  14   // for hardware SPI sclk pin (all of available pins)

//ИК приёмник VS1838
#define IR_DATA   33
#define IR_TOUCH  32
//i2s цап+амп max98357a
#define DAC_LRCK  25
#define DAC_BCLK  26
#define DAC_DATA  22
//led strip 
#define LED_DATA  23
#define NUM_LEDS 8
//таймеры
#define PERIOD_1 10000    // перерыв между включением диодов (мс)
#define PERIOD_2 100000      // время работы диодов
#define PERIOD_3 600000
#define PERIOD_4 1000

#define BLACK 0x0000
#define WHITE 0xFFFF



//инициализируем оборудование
Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);

//инициализируем дисплей
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//определяем массив led
CRGB leds[NUM_LEDS]; 

//константы для вайфая
const char *SSID = "Tensor"; // имя точки
const char *PASSWORD = "87654321"; // пароль

//для таймеров
unsigned long timer_1, timer_2, timer_3, timer_4;
const float p = 3.1415926;
uint32_t targetTime = 0;

//============================================================================================================================================
void setup(void) {
  Serial.begin(9600);
  enc1.setType(TYPE1);  // initialize encoder
  FastLED.addLeds<NEOPIXEL, LED_DATA>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  WiFi.begin(SSID, PASSWORD);
  timer_1 = millis();
  timer_2 = millis();
  timer_3 = millis();
  timer_4 = millis();


  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  targetTime = millis() + 1000;
  }
//==============================================================================================================================================

void loop() {

  enc1.tick(); // обязательная функция отработки. Должна постоянно опрашиваться
  
  //===============Первый таймер=============================

  if (millis() - timer_1 > PERIOD_1) {
    timer_1 = millis();                   // сброс таймера
    
    }
  if (millis() - timer_2 > PERIOD_2) {
    timer_2 = millis();
    
    }
  //=========================================================

  
  //===============Второй таймер для leds====================
  if (millis() - timer_3 > PERIOD_3) {  //заходим внутрь, если прошло три секунды (PERIOD_2) со старта всего loop
    timer_3 = millis();                //приравниваем timer_2 к текущиму времени, после чего
    fill_solid(leds, NUM_LEDS, CRGB::Aquamarine);
    FastLED.show();
    }

  if (millis() - timer_4 > PERIOD_4) {
    timer_4 = millis();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    }
  selfperpetuating(2);      
}

void selfperpetuating(uint16_t renew) {
  tft.setFreeFont(FF18);
}