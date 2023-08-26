//#include <avr/pgmspace.h>
#include "cat.h"
#include "background.h"
#include "component.h"
#include "graphic16x16.h"
#include "FastLED.h"     
#define NUM_LEDS 256
#define DATA_PIN 14
CRGB leds[NUM_LEDS];
Graphic16x16 graphic;
unsigned int bright = 50;
time_t sec_time;
void setup() { 
  FastLED.addLeds<WS2812B,DATA_PIN, GBR>(leds, NUM_LEDS);  // Init of the Fastled library
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  FastLED.setBrightness(bright);
  Serial.begin(115200);
  randomSeed((unsigned) time(&sec_time));
  graphic.setBackground(background_data[0]);
}
unsigned long last_time, ignore_time;
uint8_t behave;
uint8_t frame, rand_frame;
uint8_t menu = 0;
uint8_t mode;
bool food;
void loop() { 
  // Serial.print(digitalRead(32));
  // Serial.print(digitalRead(33));
  // Serial.print(digitalRead(34));
  // Serial.println(digitalRead(35));
  size_t freeHeap = ESP.getFreeHeap();
  Serial.print("Free Heap Memory: ");
  Serial.print(freeHeap);
  Serial.println(" bytes");
  if((millis() - ignore_time) > 10000){
    mode = 1;
    if(food){
      if((behave == 4 && frame > 15) || (behave == 5 && frame > 15)){
        behave = behave == 4 ? 6 : 7;
        rand_frame = 14;
        frame = 0;
      }else if(frame == rand_frame){
        if(behave == 6 || behave == 7){
          behave = 8;
          rand_frame = 50;
          frame = 0;
        }else{
          food = false;
        }
      }else if(behave != 8 && behave != 6 && behave != 7){
        behave = 8;
        rand_frame = 50;
        frame = 0;
      }
    }

    if((frame == rand_frame) && !food){
      if(behave == 4 || behave == 5){
        behave = behave == 4 ? 6 : 7;
        rand_frame = 16;
      }else if(behave == 6 || behave == 7){
        behave = behave == 6 ? 3 : 2;
        rand_frame = random(100);
      }else{
        behave = random(6);
        rand_frame = random(100) + 50;
      }
      frame = 0;
    }
    switch(behave){
      case 0 : graphic.draw(cat_sit_data[frame % CAT_SIT_FRAME], 0, 0); break;
      case 1 : graphic.draw(cat_sit_data[frame % CAT_SIT_FRAME], 0, 0); break;
      case 2 : graphic.draw(cat_stand_r_data[frame % CAT_STAND_R_FRAME], 0, 0); break;
      case 3 : graphic.draw(cat_stand_l_data[frame % CAT_STAND_L_FRAME], 0, 0); break;
      case 4 :
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(cat_walk_r_data[frame], 0, 0);
                else
                  graphic.draw(cat_walk_r_data[5 + (frame % 4)], frame - 5, 0);
              }else{
                graphic.draw(background_data[0], 0, 0);
              }
              break;
      case 5 : 
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(cat_walk_l_data[frame], 0, 0);
                else
                  graphic.draw(cat_walk_l_data[5 + (frame % 4)], -(frame - 5) - 1, 0);
              }else{
                graphic.draw(background_data[0], 0, 0);
              }
              break;
      case 6 : graphic.draw(cat_walk_back_r_data[frame % CAT_WALK_BACK_R_FRAME], (16 - frame), 0); break;
      case 7 : graphic.draw(cat_walk_back_l_data[frame % CAT_WALK_BACK_L_FRAME], -(16 - frame), 0); break;
      // case 8 : display_frame(cat_eat_data[frame % 4]); break;
    }
    display();
    frame++;
    delay(150);
   }else{ 
     if(mode == 0){
       graphic.draw(menu_icon_data[menu], 0, 0);
     }else if(mode == 2){
       graphic.draw(status_data[menu], 0, 0);
     }
     display();
   }
    //for(int frame = 0; frame < 6; frame++){
      if((millis() - last_time) > 150){
        last_time = millis();
        if(!digitalRead(32)){
          if(mode != 2){
            mode = 2;
            menu = 0;
          }else{
            menu < 2 ? menu++ : menu = 0;
          }
          ignore_time = millis();
        }
        if(!digitalRead(33)){
          if(!mode){
            menu < 5 ? menu++ : menu = 0;
            ignore_time = millis();
          }
        }
        if(!digitalRead(34)){
          if(!mode){
            menu > 0 ? menu-- : menu = 5;
            ignore_time = millis();
          }else{
            food = true;
          }
        }
        if(!digitalRead(35)){
          mode = 0;
          menu = 0;
          ignore_time = millis();
        }
      }
}

void display_frame(uint32_t const frame_data[256]){
  int count = 0;
  FastLED.clear();
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      leds[graphic.XY(j, i)] = frame_data[count];
      count++;
    }
  }
  FastLED.show(); 
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

