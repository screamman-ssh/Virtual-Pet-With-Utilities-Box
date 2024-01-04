//#include <avr/pgmspace.h>
#include "cat.h"
#include "isom_cat.h"
#include "idum_cat.h"
#include "background.h"
#include "digit.h"
#include "component.h"
#include "graphic16x16.h"
#include  <DHT.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <string.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <esp_task_wdt.h>
#define DHTPIN 23        
#define DHTTYPE DHT11 
#define WDT_TIMEOUT 5    
DHT dht(DHTPIN, DHT11);            //กำหนดค่าเริ่มต้นให้ DHT11
tmElements_t tm; 
Graphic16x16 graphic;              //สร้าง Object จาก Graphic16x16
HardwareSerial SerialPort(2);      //เปิดใช้งาน UART2
unsigned int bright = 50;
time_t sec_time;
uint8_t main_menu = 6;
uint8_t mode = 1;
unsigned long last_time, ignore_time;
uint8_t frame, behave;
uint8_t spec_behave, update_behave;
float energyStatus, loveStatus, happyStatus;
uint8_t background, cat_skin;
uint8_t digit_clock_color, bg_clock_color;
uint8_t calendar_mode;
uint8_t weather_status;
uint32_t const (*sel_cat_love)[256];
uint32_t const (*sel_cat_sleep)[256];
uint32_t const (*sel_cat_lay)[256];
uint32_t const (*sel_cat_eat)[256];
uint32_t const (*sel_cat_sit)[256];
uint32_t const (*sel_cat_stand_l)[256];
uint32_t const (*sel_cat_stand_r)[256];
uint32_t const (*sel_cat_walk_l)[256];
uint32_t const (*sel_cat_walk_r)[256];
uint32_t const (*sel_cat_walk_back_l)[256];
uint32_t const (*sel_cat_walk_back_r)[256];

void setup() { 
  //กำหนดการทำงานของขาที่ใช้เป็นอินพุต
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(27, INPUT);
  //กำหนดค่าให้กับการเชื่อมต่อเเบบ Serial
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  //กำหนด Seed ให้กับฟังก์ชัน random
  randomSeed((unsigned) time(&sec_time));
  //กำหนดค่าสถานะเริ่มต้นต่างๆเเละเปิดใช้งานไลบรารี่ให้กับโปรเเกรม
  energyStatus = 100;
  loveStatus = 100;
  happyStatus = 100;
  update_behave = true;
  graphic.setup();
  graphic.setBackground(background_data[0]);
  dht.begin(); 
  Wire.begin();
  //เริ่มต้นการใช้งานเเละอ่านค่าจาก EEPROM
  EEPROM.begin(512);
  uint8_t t = EEPROM.read(0);
  select_cat_skin(t & 0x0F);
  background = t >> 4;
  Serial.println(t, HEX);
  t = EEPROM.read(1);
  digit_clock_color = t & 0x0F;
  bg_clock_color = t >> 4;
  t = EEPROM.read(2);
  calendar_mode = t;
  //เริ่มต้นการใช้งานเเละกำหนดค่าให้ Watchdog
  esp_task_wdt_init(WDT_TIMEOUT, true); //WDT_TIMROUT = 5
  esp_task_wdt_add(NULL);
  //เริ่มเเสดงหน้าโหมด
  ignore_time = millis() + 10100;
}

void loop() { 
  // size_t freeHeap = ESP.getFreeHeap();
  // Serial.print("Free Heap Memory: ");
  // Serial.print(freeHeap);
  // Serial.println(" bytes");
  //Serial.println(energyStatus);

  //กำหนดค่าความสว่างให้กับ LED
  bright = int((analogRead(27)/4095.0) * 100);
  bright > 1 ? : bright = 1;
  graphic.setBrightness(bright);
  //เลือกการทำงานในโหมดต่างๆเเละเเสดงผล
  graphic.clear();
  if((millis() - ignore_time) > 10000 || mode != 0){
    esp_task_wdt_reset();
    switch(main_menu){
      case 0 : display_clock();break;
      case 1 : display_temp();break;
      case 2 : display_calendar();break;
      case 3 : display_snake_game();break;
      case 4 : display_change_skin_pet();break;
      case 5 : display_change_background_pet();break;
      case 6 : display_pet();break;
      default : display_pet();
    }
  }else{ 
    esp_task_wdt_reset();
    if(mode == 0){
      graphic.draw(menu_icon_data[main_menu], 0, 0);
    }
    graphic.display();
  }
  //อ่านค่าจากปุ่มกด 4 ปุ่ม
  if((millis() - last_time) > 150){
    last_time = millis();
    if(mode == 0){
      if(!digitalRead(33)){
        if(mode == 0){
          main_menu < 6 ? main_menu++ : main_menu = 0;
          ignore_time = millis();
          while(!digitalRead(33));
          Serial.println(main_menu);
        }
      }
      if(!digitalRead(34)){
        if(mode == 0){
          main_menu > 0 ? main_menu-- : main_menu = 6;
          ignore_time = millis();
          while(!digitalRead(34));
          Serial.println(main_menu);
        }
      }
    }
    if(mode != 2){
      if(!digitalRead(35)){
        if(mode != 0){
          mode = 0;
          ignore_time = millis();
        }else if(mode == 0){
          ignore_time += 10000;
          mode = 1;
          Serial.print("Current Page : ");
          Serial.println(main_menu);
        }
      }
      while(!digitalRead(35));
    }
  }
}

//กำหนดตำเเหน่งหน่วยของจำของข้อมูลชนิดเเมวที่จะใช้เเสดงผล
void select_cat_skin(uint8_t skin){
  if(skin == 0){
    sel_cat_love = cat_love_data;
    sel_cat_sleep = cat_sleep_data;
    sel_cat_lay = cat_lay_data;
    sel_cat_eat = cat_eat_data;
    sel_cat_sit = cat_sit_data;
    sel_cat_stand_l = cat_stand_l_data;
    sel_cat_stand_r = cat_stand_r_data;
    sel_cat_walk_l = cat_walk_l_data;
    sel_cat_walk_r = cat_walk_r_data;
    sel_cat_walk_back_l = cat_walk_back_l_data;
    sel_cat_walk_back_r = cat_walk_back_r_data;
  }else if(skin == 1){
    sel_cat_love = isom_cat_love_data;
    sel_cat_sleep = isom_cat_sleep_data;
    sel_cat_lay = isom_cat_lay_data;
    sel_cat_eat = isom_cat_eat_data;
    sel_cat_sit = isom_cat_sit_data;
    sel_cat_stand_l = isom_cat_stand_l_data;
    sel_cat_stand_r = isom_cat_stand_r_data;
    sel_cat_walk_l = isom_cat_walk_l_data;
    sel_cat_walk_r = isom_cat_walk_r_data;
    sel_cat_walk_back_l = isom_cat_walk_back_l_data;
    sel_cat_walk_back_r = isom_cat_walk_back_r_data;
  }else if(skin == 2){
    sel_cat_love = idum_cat_love_data;
    sel_cat_sleep = idum_cat_sleep_data;
    sel_cat_lay = idum_cat_lay_data;
    sel_cat_eat = idum_cat_eat_data;
    sel_cat_sit = idum_cat_sit_data;
    sel_cat_stand_l = idum_cat_stand_l_data;
    sel_cat_stand_r = idum_cat_stand_r_data;
    sel_cat_walk_l = idum_cat_walk_l_data;
    sel_cat_walk_r = idum_cat_walk_r_data;
    sel_cat_walk_back_l = idum_cat_walk_back_l_data;
    sel_cat_walk_back_r = idum_cat_walk_back_r_data;
  }
}

//โหมดเลี้ยงเเมว
void display_pet(){
  static uint8_t rand_frame;
  static uint8_t menu;
  static unsigned long status_time;
  static bool status;
  //อัพเดตเเละรับข้อมูลผ่าน Odroid-C4 ที่ติดต่่อฐานข้อมูลหรือ Google Sheet
  if((millis() - update_behave) > 500){
    update_behave = millis();
    update_data_to_odroid(0);
  }
  //อ่านค่าจากปุ่มกด
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
  if(!digitalRead(33)){
    if(mode != 0){
      spec_behave = 2;
    }
  }
  if(!digitalRead(34)){
    if(mode != 0){
      spec_behave = 1;
    }
  }
  //จับเวลาการเเสดงผลหน้าจอเเสดงสถานะของเเมว
  if((millis() - status_time) > 5000)
    status = false;

  //เเสดงผลเเมวในกริยาต่างๆเเละเปลี่ยนเเปลงค่าสถานะของเเมว
  if(!status){
    //กำหนดฉากพื้นหลัง
    graphic.setBackground(background_data[background]);
    //ตรวจสอบว่าเป็นกริยาของการโต้ตอบหรือไม่ (การกินเเละการมอบความรัก)
    if(spec_behave){
      if((behave == 4 && frame > 15) || (behave == 5 && frame > 15)){
        behave = behave == 4 ? 6 : 7;
        rand_frame = 14;
        frame = 0;
      }else if(frame == rand_frame){
        if(behave == 6 || behave == 7){
          behave = spec_behave == 1 ? 8 : 9;
          rand_frame =60;
          frame = 0;
        }else{
          spec_behave == 2 ? loveStatus = 100 : energyStatus = 100;
          spec_behave = 0;
        }
      }else if(behave != 8 && behave != 9 && behave != 6 && behave != 7){
        behave = spec_behave == 1 ? 8 : 9;
        rand_frame =60;
        frame = 0;
      }
    }
    //เปลี่ยนเเปลงกริยาเเละระยะเวลาในการเเสดงผลกริยานั้นๆ
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
    //ตรวจสอบว่าค่าสถานะพลังงานเป็น 0 เเล้วหรือยัง
    if(energyStatus == 0 & behave != 8){
      //ถ้าใช่ ก็ให้เเมวเเสดงกริยาหลับ
      behave = 10;
      rand_frame += 1;
    }
    //เเสดงผลเเมวตามกริยาที่ได้ถูกสุ่มหรือถูกกำหนด
    switch(behave){
      case 0 : graphic.draw(sel_cat_sit[frame % CAT_SIT_FRAME], 0, 0); break;
      case 1 : graphic.draw(sel_cat_lay[frame % CAT_LAY_FRAME], 0, 0); break;
      case 2 : graphic.draw(sel_cat_stand_r[frame % CAT_STAND_R_FRAME], 0, 0); break;
      case 3 : graphic.draw(sel_cat_stand_l[frame % CAT_STAND_L_FRAME], 0, 0); break;
      case 4 :
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(sel_cat_walk_r[frame], 0, 0);
                else
                  graphic.draw(sel_cat_walk_r[5 + (frame % 4)], frame - 5, 0);
              }
              break;
      case 5 : 
              if(frame < 16){
                if(frame < 5)
                  graphic.draw(sel_cat_walk_l[frame], 0, 0);
                else
                  graphic.draw(sel_cat_walk_l[5 + (frame % 4)], -(frame - 5) - 1, 0);
              }
              break;
      case 6 : graphic.draw(sel_cat_walk_back_r[frame % CAT_WALK_BACK_R_FRAME], (16 - frame), 0); break;
      case 7 : graphic.draw(sel_cat_walk_back_l[frame % CAT_WALK_BACK_L_FRAME], -(16 - frame), 0); break;
      case 8 : graphic.draw(sel_cat_eat[frame % CAT_EAT_FRAME], 0, 0); break;
      case 9 : graphic.draw(sel_cat_love[frame % CAT_LOVE_FRAME], 0, 0); break;
      case 10 : graphic.draw(sel_cat_sleep[frame % CAT_SLEEP_FRAME], 0, 0); break;
    }
    frame++;
    //เปลี่ยนเเปลงค่าสถานะต่างๆ
    energyStatus > 0 ? energyStatus -= ((loveStatus < 50 || happyStatus < 50) ? 0.015 : 0.01 ) : energyStatus = 0;
    loveStatus > 0 ? loveStatus -= 0.008 : loveStatus = 0;
    happyStatus > 0 ? happyStatus -= 0.005 : happyStatus = 0;
    delay(150);
  }else{
    //เเสดงหน้าจอเเสดงค่าสถานะต่างๆของเเมว
    graphic.setBackground((uint8_t) 0x000000);
    graphic.draw(status_data[menu], 0, 0);
    if(menu == 0){
      if(ceil(energyStatus) == 100){
        graphic.drawWithColor(number3x5_data[1],0xffeaeaea, 5, 3, 0, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 8, 10);
      }else{
        graphic.drawWithColor(number3x5_data[(int)ceil(energyStatus) / 10],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[(int)ceil(energyStatus) % 10],0xffeaeaea, 5, 3, 8, 10);
      }
    }else if(menu == 1){
      if(ceil(happyStatus) == 100){
        graphic.drawWithColor(number3x5_data[1],0xffeaeaea, 5, 3, 0, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 8, 10);
      }else{
        graphic.drawWithColor(number3x5_data[(int)ceil(happyStatus) / 10],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[(int)ceil(happyStatus) % 10],0xffeaeaea, 5, 3, 8, 10);
      }
    }else if(menu == 2){
      if(ceil(loveStatus) == 100){
        graphic.drawWithColor(number3x5_data[1],0xffeaeaea, 5, 3, 0, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[0],0xffeaeaea, 5, 3, 8, 10);
      }else{
        graphic.drawWithColor(number3x5_data[(int)ceil(loveStatus) / 10],0xffeaeaea, 5, 3, 4, 10);
        graphic.drawWithColor(number3x5_data[(int)ceil(loveStatus) % 10],0xffeaeaea, 5, 3, 8, 10);
      }
    }

    graphic.drawWithColor(percent3x5,0xffeaeaea, 5, 3, 12, 10);
  }
  graphic.display();
}

//โหมดเปลี่ยนชนิดเเมว
void display_change_skin_pet(){
  graphic.setBackground(background_data[background]);
  if(mode != 0){
    if(!digitalRead(33)){
      cat_skin < 2 ? cat_skin++ : cat_skin = 0;
      while(!digitalRead(33));
    }
    if(!digitalRead(34)){
      cat_skin > 0 ? cat_skin-- : cat_skin = 2;
      while(!digitalRead(34));
    }
    if(!digitalRead(32)){
      behave = 2;
      main_menu = 6;
      while(!digitalRead(32));
      uint8_t cur_data = EEPROM.read(0);
      cur_data &= 0xF0;
      EEPROM.write(0, cat_skin | cur_data );
      EEPROM.commit();
    }
  }
  select_cat_skin(cat_skin);
  graphic.draw(sel_cat_stand_r[frame % CAT_STAND_R_FRAME], 0, 0);
  frame++;
  graphic.display();
  delay(150);
}
//โหมดเปลี่ยนฉากหลัง
void display_change_background_pet(){
  if(mode != 0){
    if(!digitalRead(33)){
      background < 2 ? background++ : background = 0;
      while(!digitalRead(33));
    }
    if(!digitalRead(34)){
      background > 0 ? background-- : background = 2;
      while(!digitalRead(34));
    }
    if(!digitalRead(32)){
      behave = 2;
      main_menu = 6;
      while(!digitalRead(32));
      uint8_t cur_data = EEPROM.read(0);
      cur_data &= 0x0F;
      EEPROM.write(0, (background << 4) | cur_data );
      EEPROM.commit();
    }
  }
  graphic.setBackground(background_data[background]);
  graphic.display();
  delay(150);
}
//ฟังก์ชันการติดต่อระหว่าง ESP32 กับ Odroid-C4 ผ่าน UART 
void update_data_to_odroid(uint8_t mode){
  if(mode == 0){ 
    char send_data[20];
    sprintf(send_data, "%d,%d,%d,%d,%d", (int)ceil(energyStatus), (int)ceil(loveStatus), (int)ceil(happyStatus), 0, 0);
    int e, l, h, s, f, p;
    if (SerialPort.available())
    {
      char data[100] = {};
      //ส่งค่า U เพื่อบ่งบอกให้ Odroid รู้ว่าต้องการส่งข้อมูลขึ้นไปอัพเดตบน Google Sheet
      SerialPort.write('U');
      //เเนบข้อมูลที่จะส่ง
      SerialPort.write(send_data);
      int i = 0;
      while(SerialPort.available()){
        char r = SerialPort.read();
        data[i] = r;
        i++;
      }
      sscanf(data, "%d,%d,%d,%d,%d,%d", &e, &l, &h, &s, &f, &p);
      Serial.println(data);
      //ตรวจสอบว่าค่าการกิน (f) หรือการมอบความรัก (p) เป็น 1 หรือไม่
      spec_behave = (f == 1 || p == 1) ? (f == 1 ? 1 : 2) : spec_behave;
    }
  }else if(mode == 1){
    int w;
    if (SerialPort.available())
    {
      char data[20] = {};
      //ส่งค่า W เพื่อบ่งบอกให้ Odroid รู้ว่าต้องการดึงข้อมูลสภาพอากาศ
      SerialPort.write('W');
      int i = 0;
      while(SerialPort.available()){
        char r = SerialPort.read();
        data[i] = r;
        i++;
      }
      if(data[0] == 'W'){
        Serial.println(data[1]);
        weather_status = data[1] - '0';
        Serial.println(weather_status);
      }
    }
  }
}


