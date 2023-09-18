void display_calendar(Graphic16x16 graphic) {
    graphic.setBackground((uint32_t) 0x00000000);
    graphic.drawWithColor(number3x5_data[1],0xffeaeaea,5,3,2,1);
    graphic.drawWithColor(number3x5_data[8],0xffeaeaea,5,3,6,1);
    graphic.drawWithColor(alphabet3x5_data[9],0xffeaeaea,5,3,2,8);
    graphic.drawWithColor(alphabet3x5_data[0],0xffeaeaea,5,3,6,8);
    graphic.drawWithColor(alphabet3x5_data[13],0xffeaeaea,5,3,10,8);
    delay(50);
    graphic.display();
}