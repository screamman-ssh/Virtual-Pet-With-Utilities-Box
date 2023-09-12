#include "digit.h"
#include "graphic16x16.h"
#include "FastLED.h"     
#define NUM_LEDS 256
#define DATA_PIN 14
CRGB leds[NUM_LEDS];
Graphic16x16 graphic;
unsigned int bright = 50;
time_t sec_time;
float energyStatus;

void setup() { 
  FastLED.addLeds<WS2812B,DATA_PIN, GBR>(leds, NUM_LEDS);  // Init of the Fastled library
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  FastLED.setBrightness(bright);
  //Serial.begin(115200);
  graphic.setBackground((uint32_t) 0x00000000);
}
unsigned long lasttime;
bool blink = true;
void loop() {
  graphic.clear();
  graphic.drawWithColor(number3x5_data[2],0xffeaeaea,5,3,4,2);
  graphic.drawWithColor(number3x5_data[0],0xffeaeaea,5,3,7,2);
  if((millis()-lasttime )> 1000){
    lasttime = millis();
    blink = !blink;
  }
  graphic.draw(blink? 0xffeaeaea :0x000000 ,5,7);
  graphic.draw(blink? 0xffeaeaea :0x000000 ,7,7);
  graphic.drawWithColor(number3x5_data[1],0xffeaeaea,5,3,4,9);
  graphic.drawWithColor(number3x5_data[4],0xffeaeaea,5,3,7,9);
  delay(50);
  display();
}
void display(){
  FastLED.clear();
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      leds[graphic.XY(j, i)] = graphic.MAIN_FRAME[graphic.XY(j, i)];
    }
  }
  FastLED.show(); 
}
