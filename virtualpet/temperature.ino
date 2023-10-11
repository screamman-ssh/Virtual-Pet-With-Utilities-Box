
uint32_t temp_color_array[] = {0xffbf5f00, 0xffbf0000, 0xffbf005f, 0xff6e00bc, 0xff2828e2, 0xff0b0bea};
uint8_t temp_color = 0;

//โหมดอุณหภูมิเเละสภาพอากาศ
void display_temp(){
  static uint8_t  startframe;
  //อัพเดตค่าสภาพอากาศจาก Odroid-C4
  if((millis() - update_behave) > 5000){
    update_behave = millis();
    update_data_to_odroid(1);
  }
  //อ่านค่าอุณหภูมิจากมอดูล DHT11
  int t = (int)dht.readTemperature();
  //รับค่าตำเเหน่งสีพื้นหลัง
  temp_color = temp_color_selector(t);
  //เเสดงผล
  graphic.setBackground(temp_color_array[temp_color]);
  graphic.drawWithColor(number3x5_data[t/10],0xffeaeaea,5,3,2,2);
  graphic.drawWithColor(number3x5_data[t%10],0xffeaeaea,5,3,6,2);
  if ((millis()-lasttime )> 1000){
    lasttime = millis();
    blink = !blink;
  }
  graphic.draw(blink? 0xffeaeaea : temp_color_array[temp_color] ,10,2);
  graphic.drawWithColor(alphabet3x5_data[2],0xffeaeaea,5,3,12,2);
  //เลือกอนิเมชันสัญลักษณ์เเทนสภาพอากาศตามค่า weather_status 
  switch(weather_status){
    case 0 : startframe = thunderstorm; break;
    case 1 : startframe = drizzle; break;
    case 2 : startframe = rain; break;
    case 3 : startframe = snow; break;
    case 4 : startframe = atmosphere; break;
    case 5 : startframe = clears; break;
    case 6 : startframe = clouds; break;
  }
  graphic.draw(weather_data[(frame%3)+startframe], 8, 13, 2, 8);
  frame++;
  delay(250);
  graphic.display();
}

//ฟังก์ชันกำหนดตำเเหน่งค่าสีตามช่วงอุณหภูมิต่างๆ
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
