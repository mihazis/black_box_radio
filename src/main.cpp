//Зададим включения библиотек
//При использование <> в include сначала ищет в глобальных библиотеках
//При использованиие "" в include сначала ищет в папке с текущем файлом
#include <Adafruit_GFX.h>    // Core graphics library by Adafruit
#include <Arduino_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)
#include "GyverEncoder.h"
#include "rainbow.h"
#include "tensor.h"
#include "its_all.h"
#include <FastLED.h>

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
//#import "rainbow.h" пока не понимаю зачем они тут стояли
//#import "tensor.h"
//#import "its_all.h"

//инициализируем оборудование
Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
CRGB leds[NUM_LEDS];



const float p = 3.1415926;

void setup(void) {
  
  enc1.setType(TYPE1);  // initialize encoder
  tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels
  uint16_t time = millis();
  tft.fillScreen(BLACK);
  time = millis() - time;
  delay(1000);
  //int p = 0;
  //pinMode(27, OUTPUT); пример притягивание ноги к земле
  //digitalWrite(27, LOW); //включить усилитель
  FastLED.addLeds<NEOPIXEL, LED_DATA>(leds, NUM_LEDS);
  FastLED.setBrightness(1);
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(220, 220);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop() {

  leds[0] = CRGB::Red;
  leds[1] = CRGB::Orange;
  leds[2] = CRGB::Yellow;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::DeepSkyBlue;
  leds[5] = CRGB::Blue;
  leds[6] = CRGB::Violet;
  leds[7] = CRGB::White;

  FastLED.show();

  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  
  /*if (enc1.isPress()) {     // дипслип при нажатии энкодера
  digitalWrite(27, LOW); // ваш код
  tft.fillScreen(BLACK);
  gpio_hold_en(GPIO_NUM_27);  //это чтобы в дипслипе был выклчен амп
  gpio_deep_sleep_hold_en(); //это чтобы в дипслипе был выклчен амп
  esp_deep_sleep_start(); //старт дипслипа
  }*/
  
  if (enc1.isRight()) {
    
  tft.drawRGBBitmap(0, 0, its_all, 240, 240);         // если был поворот
  //p+=1;
  //testdrawtext(p, WHITE);
  }
  
  if (enc1.isLeft())  tft.drawRGBBitmap(0, 0, rainbow, 240, 240);
  if (enc1.isRightH()) testdrawtext("Right holded", WHITE); // если было удержание + поворот
  if (enc1.isLeftH()) testdrawtext("Right holded", WHITE);
  if (enc1.isPress()) testdrawtext("Right holded", WHITE);         // нажатие на кнопку (+ дебаунс)
  if (enc1.isClick()) testdrawtext("Click", WHITE);         // отпускание кнопки (+ дебаунс)
  if (enc1.isHolded()) testdrawtext("test", WHITE);       // если была удержана и энк не поворачивался
  
  
}

