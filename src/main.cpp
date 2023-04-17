// library include
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif
#include "bouton.h"

#define PIN 26

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Bouton bt[4];
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN);
int position[2];
int start[2];
int finish[2];
int references[2][2];
int i = 0, j = 0, x, y, x_finish, y_finish, x_start, y_start;
int color_table[8] = {BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE};
int game_color_table[4] = {BLUE, RED, GREEN, BLACK};
void setup_bt(int nb_bt);                                            // initialization of the buttons
void read_bt(int nb_bt);                                             // reading of the buttons
const int bt_blue = 4, bt_yellow = 17, bt_black = 16, bt_green = 13; // buttons initiation
int button_table[4] = {bt_yellow, bt_blue, bt_black, bt_green};      // set buttons table

void setup()
{
  matrix.begin();
  matrix.setBrightness(20);
  // pins initiation
  pinMode(bt_yellow, INPUT);
  pinMode(bt_green, INPUT);
  pinMode(bt_black, INPUT);
  pinMode(bt_blue, INPUT);

  // turn off the devil led
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);

  setup_bt(4);

  do
  {
    x_finish = random(0, 7);
    y_finish = random(0, 7);
    x = random(0, 7);
    y = random(0, 7);
    x_start = x;
    y_start = y;
  } while (x_start == x_finish && y_start == y_finish);

  matrix.drawPixel(x_start, y_start, BLUE);
  matrix.drawPixel(x_finish, y_finish, RED);
  matrix.drawPixel(x, y, color_table[random(1, 7)]);
}

void loop()
{
  /*for (i = 0; i <= 7; i++)
  {
    for (j = 0; j <= 7; j++)
    {
      matrix.drawPixel(i, j, color_table[random(7,7)]);
      matrix.show();
      delay(100);
      matrix.clear();
    }
  }*/

  read_bt(4);

  if (bt[0].click() == HIGH)
  {
    matrix.drawPixel(x, y, BLACK);
    x--;
  }
  else if (bt[1].click() == HIGH)
  {
    matrix.drawPixel(x, y, BLACK);
    x++;
  }
  else if (bt[2].click() == HIGH)
  {
    matrix.drawPixel(x, y, BLACK);
    y--;
  }
  else if (bt[3].click() == HIGH)
  {
    matrix.drawPixel(x, y, BLACK);
    y++;
  }

  if (x == x_start  && y == y_start)
  {
    matrix.drawPixel(x, y, BLUE);
  }
  else
  {
    matrix.drawPixel(x, y, GREEN);
  }

  if (x == x_finish && y == y_finish)
  {
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, WHITE);
      }
    }
  }
matrix.drawPixel(x_start, y_start, BLUE);
  if (x >= 7)
    x = 7;
  if (y >= 7)
    y = 7;
  if (x <= 0)
    x = 0;
  if (y <= 0)
    y = 0;
  
   matrix.show();
}

void setup_bt(int nb_bt)
{
  /*
    Input : nb_bt : number of buttons (int)
    Output : none
    Description: This function is used to initialize the buttons
  */
  for (int k = 0; k < nb_bt; k++)
  {
    bt[k].begin(button_table[k], HIGH, 200, 1500, 200);
  }
}

void read_bt(int nb_bt)
{
  /*
    Input : nb_bt : number of buttons (int)
    Output : none
    Description: This function is used to read the buttons
  */
  for (int k = 0; k < nb_bt; k++)
  {
    bt[k].read_Bt();
  }
}