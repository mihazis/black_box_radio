//Зададим включения библиотек
//При использование <> в include сначала ищет в глобальных библиотеках
//При использованиие "" в include сначала ищет в папке с текущем файлом
#include <Arduino.h>
#include <Arduino_ST7789.h>
#include <GyverEncoder.h>
#include "rainbow.h"
#include "tensor.h"
#include "its_all.h"
#include <FastLED.h>
#include <WiFi.h>

#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

#include <Fonts/FreeMonoOblique24pt7b.h>

//в определениях указываем первоначально все используемые пины
//по принципу #define <что меняем>  <на что меняем>
//энкодер KY-040
#define ENC_CLK   17
#define ENC_DT    16
#define ENC_SW    5
//дисплей ST7789 240*240 SPI
#define TFT_DC    2
#define TFT_RST   4 
#define TFT_MOSI  13   // for hardware SPI data pin (all of available pins)
#define TFT_SCLK  14   // for hardware SPI sclk pin (all of available pins)
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
#define PERIOD_1 600000    // перерыв между включением диодов (мс)
#define PERIOD_2 1000      // время работы диодов
#define PERIOD_3 66611111

//инициализируем оборудование
Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
//определяем массив led
CRGB leds[NUM_LEDS]; 
//константы для вайфая
const char *SSID = "Tensor"; // имя точки
const char *PASSWORD = "87654321"; // пароль
//для таймеров
unsigned long timer_1, timer_2, timer_3;

const float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  enc1.setType(TYPE1);  // initialize encoder
  tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels
  uint16_t time = millis();
  delay(1000);
  tft.fillScreen(BLACK);
  time = millis() - time;
  
  FastLED.addLeds<NEOPIXEL, LED_DATA>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  WiFi.begin(SSID, PASSWORD);
  }

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(25, 120);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setFont(&FreeMonoOblique24pt7b);
  tft.print(text);
}

void loop() {

  enc1.tick(); // обязательная функция отработки. Должна постоянно опрашиваться

  //===============Первый таймер=============================

  if (millis() - timer_1 > PERIOD_1) {
    timer_1 = millis();                   // сброс таймера
    fill_solid(leds, NUM_LEDS, CRGB::Aquamarine);
    FastLED.show();
    testdrawtext("ZABBIX", RED);
    
  }
  if (millis() - timer_1 > PERIOD_2) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    tft.fillScreen(BLACK);
  }
  //=========================================================

      
  //===============Второй таймер=============================
  if (millis() - timer_2 > PERIOD_2) {
    timer_2 = millis();
    //tft.fillScreen(BLACK);
    
  }
  //=========================================================


  //===============Третий таймер=============================
  if (millis() - timer_3 > PERIOD_3) {
    timer_3 = millis();
        
  }
  //=========================================================
  
}











  /*
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Orange;
  leds[2] = CRGB::Yellow;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::DeepSkyBlue;
  leds[5] = CRGB::Blue;
  leds[6] = CRGB::Violet;
  leds[7] = CRGB::White;
  */

 /*
    if (enc1.isRight()) {
    tft.drawRGBBitmap(0, 0, its_all, 240, 240);         // если был поворот
    }
    
    if (enc1.isLeft())  tft.drawRGBBitmap(0, 0, rainbow, 240, 240);
    if (enc1.isRightH()) testdrawtext("Right holded", WHITE); // если было удержание + поворот
    if (enc1.isLeftH()) testdrawtext("Right holded", WHITE);
    if (enc1.isPress()) testdrawtext("Right holded", WHITE);         // нажатие на кнопку (+ дебаунс)
    if (enc1.isClick()) testdrawtext("Click", WHITE);         // отпускание кнопки (+ дебаунс)
    if (enc1.isHolded()) testdrawtext("test", WHITE);       // если была удержана и энк не поворачивался
 */


/*
 tft.init(240, 240); //инициализация экрана, задаем его размер

tft.fillScreen(BLACK); //закрасить черным


tft.setCursor(0,0); //установить курсор

tft.setTextColor(GREEN); //цвет текста

tft.setTextSize(0); //размер от 1 до 5

tft.print("Angle:"); //вывод текста

tft.println("Angle:"); //вывод текста, новый текст будет с новой строки

tft.drawLine(x1, y1,x2, y2, color); //линия

tft.drawRect(x1, y1 ,x2, y2, color); //прямоугольник

tft.fillRect(x1, y1 ,x2, y2, color); //прямоугольник закрашенный

tft.drawCircle(x, y, radius, color); //круг

tft.fillCircle(x, y, radius, color); //круг заполненный
*/

/*
  if (enc1.isPress()) {     // дипслип при нажатии энкодера
  digitalWrite(27, LOW); // ваш код
  tft.fillScreen(BLACK);
  gpio_hold_en(GPIO_NUM_27);  //это чтобы в дипслипе был выклчен амп
  gpio_deep_sleep_hold_en(); //это чтобы в дипслипе был выклчен амп
  esp_deep_sleep_start(); //старт дипслипа
  }
  */