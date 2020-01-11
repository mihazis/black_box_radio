#include <Adafruit_GFX.h>    // Core graphics library by Adafruit
#include <Arduino_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)

#include "GyverEncoder.h"

#define CLK 17
#define DT 16
#define SW 5
Encoder enc1(CLK, DT, SW);
#define TFT_DC    2
#define TFT_RST   4 
#define TFT_MOSI  13   // for hardware SPI data pin (all of available pins)
#define TFT_SCLK  14   // for hardware SPI sclk pin (all of available pins)
#define AMP_SHTDWN  27   // amp shutdown pin (need to be pinned to GND to off)
#define DAC_OUT  25   // output dac1


#import "rainbow.h"
#import "tensor.h"
#import "its_all.h"

//You can use different type of hardware initialization
//using hardware SPI (11, 13 on UNO; 51, 52 on MEGA; ICSP-4, ICSP-3 on DUE and etc)
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); //for display without CS pin
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS); //for display with CS pin
//or you can use software SPI on all available pins (slow)
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_CS); //for display with CS pin

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  enc1.setType(TYPE1);  // initialize encoder
  tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels
  uint16_t time = millis();
  tft.fillScreen(BLACK);
  time = millis() - time;
  delay(1000);
  int p = 0;
  pinMode(27, OUTPUT);
  digitalWrite(27, LOW); //включить усилитель
  //digitalWrite(27, LOW); //выключить усилитель
  
}
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(220, 220);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
void loop() {

  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  
  if (enc1.isPress()) {     // если был совершён поворот (индикатор поворота в любую сторону)
  digitalWrite(27, LOW); // ваш код
  tft.fillScreen(BLACK);
  gpio_hold_en(GPIO_NUM_27);  //это чтобы в дипслипе был выклчен амп
  gpio_deep_sleep_hold_en(); //это чтобы в дипслипе был выклчен амп
  esp_deep_sleep_start(); //старт дипслипа
  }
  
  if (enc1.isRight()) {
    
  tft.drawRGBBitmap(0, 0, its_all, 240, 240);         // если был поворот
  //p+=1;
  //testdrawtext(p, WHITE);
  }
  
  if (enc1.isLeft())  tft.drawRGBBitmap(0, 0, rainbow, 240, 240);
  
  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Right holded");
  
  if (enc1.isPress()) Serial.println("Right holded");         // нажатие на кнопку (+ дебаунс)
  if (enc1.isClick()) Serial.println("Click");         // отпускание кнопки (+ дебаунс)
  //if (enc1.isRelease()) Serial.println("Release");     // то же самое, что isClick
  
  if (enc1.isHolded()) testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat.", WHITE);       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки
 
}



