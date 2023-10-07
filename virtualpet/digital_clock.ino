
unsigned long lasttime;
bool blink = true;
uint32_t color[] = {0x00000000, 0xffeaeaea, 0xff0000bf, 0xffbf5f00, 0xff62b217, 0xff007fff}; 

void display_clock(Graphic16x16 graphic){

  read_sw_digital_clock();
  if(RTC.read(tm)){
    graphic.setBackground(color[bg_clock_color]);
    graphic.drawWithColor(number3x5_data[tm.Hour/10],color[digit_clock_color],5,3,4,1);
    graphic.drawWithColor(number3x5_data[tm.Hour%10],color[digit_clock_color],5,3,8,1);
    
    if((millis()-lasttime )> 1000){
      lasttime = millis();
      blink = !blink;
    }
    graphic.draw(blink? color[digit_clock_color] :color[bg_clock_color] ,6,7);
    graphic.draw(blink? color[digit_clock_color] :color[bg_clock_color] ,8,7);
    graphic.drawWithColor(number3x5_data[tm.Minute/10],color[digit_clock_color],5,3,4,9);
    graphic.drawWithColor(number3x5_data[tm.Minute%10],color[digit_clock_color],5,3,8,9);
    delay(50);
    graphic.display();
  }
  
}

void read_sw_digital_clock(){
  if(!digitalRead(32)){
    if((millis() - last_time) > 100){
        last_time = millis();
        while(!digitalRead(32));
        digit_clock_color < 6 ? digit_clock_color++ : digit_clock_color = 0;
        digit_clock_color != bg_clock_color ? : digit_clock_color < 6 ? digit_clock_color++ : digit_clock_color = 0;
        uint8_t cur_data = EEPROM.read(1);
        cur_data &= 0xF0;
        EEPROM.write(1, digit_clock_color | cur_data );
        EEPROM.commit();
    }
  }
  if(!digitalRead(33)){
    if((millis() - last_time) > 100){
        last_time = millis();
        while(!digitalRead(33));
        bg_clock_color < 6 ? bg_clock_color++ : bg_clock_color = 0;
        digit_clock_color != bg_clock_color ? : bg_clock_color < 6 ? bg_clock_color++ : bg_clock_color = 0;
        uint8_t cur_data = EEPROM.read(1);
        cur_data &= 0x0F;
        EEPROM.write(1, (bg_clock_color << 4) | cur_data );
        EEPROM.commit();
    }
  }
}
