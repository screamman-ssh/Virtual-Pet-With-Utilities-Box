#ifndef GRAPHIC16X16_H_
#define GRAPHIC16X16_H_
#include "Arduino.h"

class Graphic16x16{
  private:
    uint32_t BACKGROUND[256];
  public:
    uint32_t MAIN_FRAME[256];
    uint8_t XY(uint8_t x, uint8_t y);
    void setBackground(uint32_t const background[256]);
    void setBackground(uint32_t color);
    void draw(uint32_t const object[256], int8_t x, int8_t y);
    void clear();
};

#endif