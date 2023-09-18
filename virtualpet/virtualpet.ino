//#include <avr/pgmspace.h>
#include "cat.h"
#include "isom_cat.h"
#include "background.h"
#include "digit.h"
#include "component.h"
#include "graphic16x16.h"
Graphic16x16 graphic;
time_t sec_time;
float energyStatus;

void setup() { 
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  Serial.begin(115200);
  randomSeed((unsigned) time(&sec_time));
  energyStatus = 100;
  graphic.setup();
  graphic.setBackground(background_data[0]);
}
unsigned long last_time, ignore_time;
uint8_t behave, spec_behave;
uint8_t frame, rand_frame;
uint8_t main_menu = 0;
uint8_t mode;

void loop() { 
  size_t freeHeap = ESP.getFreeHeap();
  Serial.print("Free Heap Memory: ");
  Serial.print(freeHeap);
  Serial.println(" bytes");
  //Serial.println(energyStatus);

  graphic.clear();
  if((millis() - ignore_time) > 10000){
    switch(main_menu){
      case 0 : display_clock(graphic);break;
      case 1 : display_temp(graphic);break;
      case 5 : display_pet();break;
      default : display_pet();
    }
  }else{ 
    if(mode == 0){
      graphic.draw(menu_icon_data[main_menu], 0, 0);
    }
    graphic.display();
  }
  if((millis() - last_time) > 150){
    last_time = millis();
    // if(!digitalRead(32)){
    //   ignore_time = millis();
    // }
    if(!digitalRead(33)){
      if(mode == 0){
        main_menu < 5 ? main_menu++ : main_menu = 0;
        ignore_time = millis();
      }else if(mode == 1){
        spec_behave = 2;
      }
    }
    if(!digitalRead(34)){
      if(!mode){
        main_menu > 0 ? main_menu-- : main_menu = 5;
        ignore_time = millis();
      }else{
        spec_behave = 1;
      }
    }
    if(!digitalRead(35)){
      mode = 0;
      main_menu = 0;
      ignore_time = millis();
    }
  }
}

void display_pet(){
  static uint8_t menu;
  static unsigned long status_time;
  static bool status;
  if(!digitalRead(32)){
    if(!status){
      status = true;
      menu = 0;
    }else{
      if((millis() - last_time) > 50){
        menu < 2 ? menu++ : menu = 0;
      }
    }
    last_time = millis();
    status_time = millis();
  }

  if((millis() - status_time) > 5000)
    status = false;

  if(!status){
    if(mode != 1){
      mode = 1;
    }
    graphic.setBackground(background_data[0]);
    if(spec_behave){
      if((behave == 4 && frame > 15) || (behave == 5 && frame > 15)){
        behave = behave == 4 ? 6 : 7;
        rand_frame = 14;
        frame = 0;
      }else if(frame == rand_frame){
        if(behave == 6 || behave == 7){
          behave = spec_behave == 1 ? 8 : 9;
          rand_frame = spec_behave == 1? (100 - ceil(energyStatus)): 60;
          frame = 0;
        }else{
          spec_behave = 0;
        }
      }else if(behave != 8 && behave != 9 && behave != 6 && behave != 7){
        behave = spec_behave == 1 ? 8 : 9;
        rand_frame = spec_behave == 1? (100 - ceil(energyStatus)): 60;
        frame = 0;
      }
    }

    if((frame == rand_frame) && !spec_behave){
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

    if(energyStatus == 0 & behave != 8){
      behave = 10;
      rand_frame += 1;
    }

    switch(behave){
      case 0 : graphic.draw(isom_cat_sit_data[frame % CAT_SIT_FRAME], 0, 0); break;
      case 1 : graphic.draw(isom_cat_lay_data[frame % CAT_LAY_FRAME], 0, 0); break;
      case 2 : graphic.draw(isom_cat_stand_r_data[frame % CAT_STAND_R_FRAME], 0, 0); break;
      case 3 : graphic.draw(isom_cat_stand_l_data[frame % CAT_STAND_L_FRAME], 0, 0); break;
      case 4 :
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(isom_cat_walk_r_data[frame], 0, 0);
                else
                  graphic.draw(isom_cat_walk_r_data[5 + (frame % 4)], frame - 5, 0);
              }
              break;
      case 5 : 
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(isom_cat_walk_l_data[frame], 0, 0);
                else
                  graphic.draw(isom_cat_walk_l_data[5 + (frame % 4)], -(frame - 5) - 1, 0);
              }
              break;
      case 6 : graphic.draw(isom_cat_walk_back_r_data[frame % CAT_WALK_BACK_R_FRAME], (16 - frame), 0); break;
      case 7 : graphic.draw(isom_cat_walk_back_l_data[frame % CAT_WALK_BACK_L_FRAME], -(16 - frame), 0); break;
      case 8 : graphic.draw(isom_cat_eat_data[frame % CAT_EAT_FRAME], 0, 0); energyStatus < 100 ? energyStatus += 1 : energyStatus = 100; break;
      case 9 : graphic.draw(isom_cat_love_data[frame % CAT_LOVE_FRAME], 0, 0); break;
      case 10 : graphic.draw(isom_cat_sleep_data[frame % CAT_SLEEP_FRAME], 0, 0); break;
    }
    frame++;
    energyStatus > 0 ? energyStatus -= 0.001 : energyStatus = 0;
    delay(150);
  }else{
    graphic.setBackground((uint8_t) 0x000000);
    graphic.draw(status_data[menu], 0, 0);
    if(ceil(energyStatus) == 100){
      graphic.drawWithColor(number3x5_data[1],0xffeaeaea, 5, 3, 0, 10);
      graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 4, 10);
      graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 8, 10);
    }else{
      graphic.drawWithColor(number3x5_data[(int)ceil(energyStatus) / 10],0xffeaeaea, 5, 3, 4, 10);
      graphic.drawWithColor(number3x5_data[(int)ceil(energyStatus) % 10],0xffeaeaea, 5, 3, 8, 10);
    }

    graphic.drawWithColor(percent3x5,0xffeaeaea, 5, 3, 12, 10);
  }
  graphic.display();
}


