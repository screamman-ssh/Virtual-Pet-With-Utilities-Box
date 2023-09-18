void display_calendar(Graphic16x16 graphic) {
    graphic.setBackground(0xffeaeaea);
    graphic.drawWithColor(number3x5_data[1],0xffeaeaea,5,3,1,1);
    graphic.drawWithColor(number3x5_data[8],0xffeaeaea,5,3,5,1);
    graphic.drawWithColor(alphabet3x5_data[9],0xffeaeaea,5,3,1,8);
    graphic.drawWithColor(alphabet3x5_data[0],0xffeaeaea,5,3,5,8);
    graphic.drawWithColor(alphabet3x5_data[13],0xffeaeaea,5,3,9,8);
}