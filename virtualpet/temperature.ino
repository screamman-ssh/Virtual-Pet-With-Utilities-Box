
void display_temp(Graphic16x16 graphic){
  graphic.setBackground((uint32_t) 0x00000000);
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