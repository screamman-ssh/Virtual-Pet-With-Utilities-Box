uint8_t x[20],y[20];
int d_y,d_x=1;
uint8_t food_empty=1;
uint8_t food_x,food_y;
uint8_t snake_len = 1;
uint8_t alive = 1;

void display_snake_game(){
  graphic.clear();
  read_sw();
  if (food_empty){
    spawn_food();
  }
  if ((x[0]+d_x)<0 || (x[0]+d_x)>15)  alive=0;
  if ((y[0]+d_y)<0 || (y[0]+d_y)>15)  alive=0;
  
  graphic.setBackground((uint32_t)0);
  graphic.draw(0xff0000bf, food_x, food_y);
  if(alive){
    move_snake();
    for(int i = 0; i < snake_len; i++)
      graphic.draw(0xff055405, x[i], y[i]);
  }
  else{
    for(int n = 0; n < 3; n++){
      for(int i = 0; i < snake_len; i++)
        graphic.draw(0xff055405, x[i], y[i]);
      graphic.display();
      delay(300);
      for(int i = 0; i < snake_len; i++)
        graphic.draw((uint32_t)0, x[i], y[i]);
      graphic.display();
      delay(300);
    }
  }
  
  graphic.display();
  delay(150); 

  Serial.print(d_x);
  Serial.println(d_y);
  
}


void read_sw(){
  if(!digitalRead(32)){
    if(d_x == 0){
      d_x = 1;
    }
    d_y=0;
  }
  if(!digitalRead(33)){
    if(d_x == 0){
      d_x = -1;
    }
    d_y=0;
  }
  if(!digitalRead(35)){
    if(d_y == 0){
      d_y = -1;
    }
    d_x=0;
  }
  if(!digitalRead(34)){
    if(d_y == 0){
      d_y = 1;
    }
    d_x=0;
  }
}

void move_snake(){
  uint8_t x_head,y_head;
  uint8_t x_tail[20],y_tail[20];
  
  for(int i=0; i<20; i++){
    x_tail[i] = x[i];
    y_tail[i] = y[i];
  }
  x_head = x[0];
  y_head = y[0];
  for(int i = 1; i < snake_len; i++){
    x[i] = x_tail[i-1];
    y[i] = y_tail[i-1];
  }
  x_head += d_x;
  y_head += d_y;
  if ((food_x == x_head && food_y == y_head)){
    snake_len<20? snake_len++: snake_len = 20;
    spawn_food();
    for(int i = 1; i < snake_len; i++){
      x[i] = x_tail[i-1];
      y[i] = y_tail[i-1];
    }   
  }
  x[0] = x_head;
  y[0] = y_head;
}

void spawn_food(){
  food_x = random(15);
  food_y = random(15);
  food_empty = 0;
}
