uint32_t temp_color_array[] = {0xffbf5f00, 0xffbf0000, 0xffbf005f, 0xff00dbdb, 0xff007fff, 0xff2828e2};
uint8_t temp_color = 0;

void display_temp(Graphic16x16 graphic){
  temp_color = temp_color_selector(32);
  graphic.setBackground(temp_color_array[temp_color]);
  graphic.drawWithColor(number3x5_data[3],0xffeaeaea,5,3,2,2);
  graphic.drawWithColor(number3x5_data[2],0xffeaeaea,5,3,6,2);
  if ((millis()-lasttime )> 1000){
    lasttime = millis();
    blink = !blink;
  }
  graphic.draw(blink? 0xffeaeaea :0x000000 ,10,2);
  graphic.drawWithColor(alphabet3x5_data[2],0xffeaeaea,5,3,12,2);
  graphic.draw(temp_data[0], 6, 16, 0, 8);
  delay(50);
  graphic.display();
}

uint8_t temp_color_selector(int temp){
  uint8_t tmp;
  if (temp > -20 && temp <= 10)
    tmp = 0;
  else if (temp > 10 && temp <= 20)
    tmp = 1;
  else if (temp > 20 && temp <= 29)
    tmp = 2;
  else if (temp > 29 && temp <= 35)
    tmp = 3;
  else if (temp > 35 && temp <= 40)
    tmp = 4;
  else
    tmp = 5;
  return tmp;
}