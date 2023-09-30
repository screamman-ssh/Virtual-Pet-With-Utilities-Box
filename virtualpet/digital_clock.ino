
unsigned long lasttime;
bool blink = true;
uint32_t color[] = {0x00000000, 0xffeaeaea, 0xff0000bf, 0xffbf5f00, 0xff62b217, 0xff007fff}; 
uint8_t digit_color = 1;
uint8_t bg_color = 0;

void display_clock(Graphic16x16 graphic){
  read_sw_digital_clock();
  if(RTC.read(tm)){
    graphic.setBackground(color[bg_color]);
    graphic.drawWithColor(number3x5_data[tm.Hour/10],color[digit_color],5,3,4,2);
    graphic.drawWithColor(number3x5_data[tm.Hour%10],color[digit_color],5,3,8,2);
    
    if((millis()-lasttime )> 1000){
      lasttime = millis();
      blink = !blink;
    }
    graphic.draw(blink? color[digit_color] :color[bg_color] ,6,7);
    graphic.draw(blink? color[digit_color] :color[bg_color] ,8,7);
    graphic.drawWithColor(number3x5_data[tm.Minute/10],color[digit_color],5,3,4,9);
    graphic.drawWithColor(number3x5_data[tm.Minute%10],color[digit_color],5,3,8,9);
    delay(50);
    graphic.display();
  }
  
}

void read_sw_digital_clock(){
  if(!digitalRead(32)){
    if((millis() - last_time) > 100){
        last_time = millis();
        while(!digitalRead(32));
        digit_color < 6 ? digit_color++ : digit_color = 0;
        digit_color != bg_color ? : digit_color < 6 ? digit_color++ : digit_color = 0;
    }
  }
  if(!digitalRead(33)){
    if((millis() - last_time) > 100){
        last_time = millis();
        while(!digitalRead(33));
        bg_color < 6 ? bg_color++ : bg_color = 0;
        digit_color != bg_color ? : bg_color < 6 ? bg_color++ : bg_color = 0;
    }
  }
}
