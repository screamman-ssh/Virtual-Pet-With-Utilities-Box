uint8_t month[12][3] = {{9, 0, 13},{5, 4, 1},{12, 0, 17}, {0, 15, 17}, {12, 0, 24}, {9, 20, 13}, {9, 20, 11}, 
                    {0, 20, 6}, {18, 4, 15}, {14, 2, 19}, {13, 14, 21}, {3, 4, 2}};

uint32_t day_color[] = {0xff0000ff, 0xff00ccff, 0xffa14bfc, 0xff119311, 0xff1278e5, 0xffe58d34, 0xffb7095d};

void display_calendar(Graphic16x16 graphic) {
    graphic.setBackground((uint32_t) 0x00000000);
    graphic.drawWithColor(number3x5_data[1],day_color[0],5,3,2,1);
    graphic.drawWithColor(number3x5_data[7],day_color[0],5,3,6,1);
    graphic.drawWithColor(alphabet3x5_data[month[10][0]],0xffeaeaea,5,3,2,8);
    graphic.drawWithColor(alphabet3x5_data[month[10][1]],0xffeaeaea,5,3,6,8);
    graphic.drawWithColor(alphabet3x5_data[month[10][2]],0xffeaeaea,5,3,10,8);
    delay(50);
    graphic.display();
}