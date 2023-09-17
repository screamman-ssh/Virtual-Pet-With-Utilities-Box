#ifndef DIGITAL_CLOCK_H_
#define DIGITAL_CLOCK_H_
#include "graphic16x16.h"

unsigned long lasttime;
bool blink = true;

void display_clock(Graphic16x16 graphic){
  graphic.setBackground((uint32_t) 0x00000000);
  graphic.drawWithColor(number3x5_data[2],0xffeaeaea,5,3,4,2);
  graphic.drawWithColor(number3x5_data[0],0xffeaeaea,5,3,8,2);
  if((millis()-lasttime )> 1000){
    lasttime = millis();
    blink = !blink;
  }
  graphic.draw(blink? 0xffeaeaea :0x000000 ,6,7);
  graphic.draw(blink? 0xffeaeaea :0x000000 ,8,7);
  graphic.drawWithColor(number3x5_data[1],0xffeaeaea,5,3,4,9);
  graphic.drawWithColor(number3x5_data[4],0xffeaeaea,5,3,8,9);
  delay(50);
  graphic.display();
}

#endif