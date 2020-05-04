//Зададим включения библиотек
//При использование <> в include сначала ищет в глобальных библиотеках
//При использованиие "" в include сначала ищет в папке с текущем файлом
#include <Arduino.h>
#include <GyverEncoder.h>
#include <FastLED.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include "dog.h"
#include <TFT_eSPI.h>                  // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

#define TEXT "aA MWyz~12"              // Text that will be printed on screen in any font
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
//#define TFT_MOSI  13                 // for hardware SPI data pin (all of available pins)
//#define TFT_SCLK  14                 // for hardware SPI sclk pin (all of available pins)

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
#define PERIOD_1 300000                // перерыв между включением диодов (мс)
#define PERIOD_2 2000               // время работы диодов

//цвета
#define BLACK 0x0000
#define WHITE 0xFFFF

//закольцовываем количество экранов
#define SCREEN_AMOUNT 32
byte screen = 1; 

const char *ssid = "Tensor";          // имя точки
const char *password = "87654321";    // пароль

Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);//инициализируем энкодер
TFT_eSPI tft = TFT_eSPI();            //инициализируем дисплей
CRGB leds[NUM_LEDS];                  //определяем массив led
WiFiUDP ntpUDP;                       //The following two lines define an NTP Client to request date and time from an NTP server.
NTPClient timeClient(ntpUDP);         //The following two lines define an NTP Client to request date and time from an NTP server.


//для таймеров
unsigned long timer_1, timer_2, timer_3, timer_4;
const float p = 3.1415926;
uint32_t targetTime = 0;
IPAddress currentip;

void setup(void) {
  Serial.begin(9600);
  enc1.setType(TYPE2);                // тип энкодера по классификации Гайвера (полушаговый энкодер)
  FastLED.addLeds<NEOPIXEL, LED_DATA>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  
  timer_1 = millis();
  timer_2 = millis();
  timer_3 = millis();
  timer_4 = millis();

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  targetTime = millis() + 1000;

  tft.setCursor(20, 25, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Connect to wi-fi...");
  tft.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.setCursor(20, 125, 4);
    tft.println("...");;
    tft.fillScreen(TFT_BLACK);
    }
  IPAddress currentip = WiFi.localIP();
  
  timeClient.begin();
  timeClient.setTimeOffset(10800);
  }

void testtext_screen(uint16_t renew) {  
  if (renew == 1) {
    tft.setCursor(0, 0, 4); //Первая цифра - двигает по горизонтали, вторая по вертикали
    tft.setTextColor(TFT_VIOLET, TFT_BLACK);
    tft.println("ABCDEFGHIJKLMNo");

    tft.setCursor(20, 25, 4);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("ABCDEFGHIJklmn");

    tft.setCursor(40, 50, 4);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.println("ABCDEFGHIJKL");

    tft.setCursor(60, 75, 4);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.println("ABCDEFGHIJK");

    tft.setCursor(80, 100, 4);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("ABCDEFGHIJ");

    tft.setCursor(100, 125, 4);
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.println("ABCDEFGH");

    tft.setCursor(120, 150, 4);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.println("ABCDEFG");

    tft.setCursor(140, 175, 4);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.println("ABCDEF");

    tft.setCursor(160, 200, 4);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("ABCD");
    
    tft.setCursor(180, 225, 4);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.println("ABC");
    } else {
      tft.fillScreen(TFT_BLACK);
      }
  }

void time_layer(byte time_conf) {      
  String formattedDate;
  String dayStamp;
  String timeStamp;
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  tft.setCursor(0, 200, 4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(timeStamp);
  } 

void ip_layer(byte ip_conf) {      
  tft.setCursor(10, 10, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(WiFi.localIP());
  } 

void screennumber_layer(byte screennumber_conf) {      
  tft.setCursor(210, 10, 2);
  tft.println(screen);
  } 

void first_timer() {
  if (millis() - timer_1 > PERIOD_1) {
    timer_1 = millis();
    uint16_t testvar1 = 1;
    fill_solid(leds, NUM_LEDS, CRGB::Crimson);
    FastLED.show();
  }
  if (millis() - timer_2 > PERIOD_2) {
    timer_2 = millis();
    uint16_t testvar2 = 1;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    }
  }

void loop() {

  enc1.tick();                          // обязательная функция опроса энкодера
  first_timer();
  time_layer(2);
  ip_layer(1);
  screennumber_layer(1);

  if (enc1.isRight()) {
    screen++;
    if (screen > SCREEN_AMOUNT) screen = 1;
    tft.fillScreen(TFT_BLACK);
  } 

  if (enc1.isLeft()) {
    screen--;
    if (screen < 1) screen = SCREEN_AMOUNT;
    tft.fillScreen(TFT_BLACK);
  }

  if (enc1.isPress()) {
    screen = 1;
    tft.fillScreen(TFT_BLACK);
  }
}

