// library include
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif
#include "bouton.h"
#include "maze.h"

#define PIN 26 // pin matrix initialization

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Bouton bt[4];                                              // buttons initialization
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN); // matrix initialization
uint16_t grey = matrix.Color(40, 40, 40);                  // initialization of a "new" color

// initialization game settings
int i = 0, j = 0, x, y, x_finish, y_finish, x_start, y_start;                 // initialization game variables
int color_table[8] = {BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE}; // initialization color table
void setup_bt(int nb_bt);                                                     // initialization of the buttons
void read_bt(int nb_bt);                                                      // reading of the buttons
const int bt_blue = 4, bt_yellow = 17, bt_black = 16, bt_green = 13;          // buttons initiation
int button_table[4] = {bt_blue, bt_yellow, bt_green, bt_black};               // set buttons table
int game_state = 0;
int number_errors = 0;

void setup()
{
  Serial.begin(9600);
  // matrix initialization
  matrix.clear();
  matrix.begin();
  // pins initiation
  pinMode(bt_yellow, INPUT);
  pinMode(bt_green, INPUT);
  pinMode(bt_black, INPUT);
  pinMode(bt_blue, INPUT);

  // turn off the devil led
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);

  // fill the matrix full white
  setup_bt(4);
  for (i = 0; i <= 7; i++)
  {
    for (j = 0; j <= 7; j++)
    {
      matrix.drawPixel(i, j, grey);
    }
  }

  matrix.setBrightness(150); // set the next LEDs in the matrix to 150 brightness

  // define the random parameters of the game
  do
  {
    x_finish = random(0, 7);
    y_finish = random(0, 7);
    x = 0; // random(0, 7);
    y = 0; // random(0, 7);
    x_start = x;
    y_start = y;
  } while (x_start == x_finish && y_start == y_finish);
}

void loop()
{
  // makes coloured ribbons
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

  read_bt(4); // the function is aptly named
  switch (game_state)
  {
  case 0: // game initialization
    matrix.drawPixel(x_start, y_start, BLUE);
    matrix.drawPixel(x_finish, y_finish, RED);
    matrix.drawPixel(x, y, color_table[random(1, 7)]);
    game_state = 1;
    break;

  case 1:
    // conditions for changing the led position
    Serial.printf("\r (%d,%d)  (%d, %d, %d, %d)", x, y, left_path_check(x, y), top_path_check(x, y), bottom_path_check(x, y), right_path_check(x, y));
    if (bt[0].click() == HIGH)
    {
      {
        if (left_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          x--;
        }
        else
        {
          game_state = 4;
        }
      }
      game_state = 1;
    }
    else if (bt[1].click() == HIGH)
    {
      {
        if (right_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          x++;
        }
        else
        {
          game_state = 4;
        }
      }
      game_state = 1;
    }
    else if (bt[2].click() == HIGH)
    {
      {
        if (bottom_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          y++;
        }
        else
        {
          game_state = 4;
        }
      }
      game_state = 1;
    }
    else if (bt[3].click() == HIGH)
    {
      {
        if (top_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          y--;
        }
        else
        {
          game_state = 4;
        }
      }
      game_state = 1;
    }

    // conditions for some colors
    if (x == x_start && y == y_start)
    {
      matrix.drawPixel(x, y, BLUE);
    }
    else
    {
      matrix.drawPixel(x, y, GREEN);
    }

    // victory condition
    if (x == x_finish && y == y_finish)
    {
      game_state = 2;
    }
    break;

  case 2:
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, WHITE);
      }
    }
    break;

  case 3:
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, RED);
      }
    }
    delay(200);
    matrix.clear();
    delay(200);
    break;

  case 4:
    number_errors++;

    if (number_errors <= 2)
      game_state = 1;

    else if (number_errors >= 3)
    {
      game_state = 3;
      // add random maze function and choose a new one
    }
    break;

  default:
    game_state = 0;
  }
  matrix.show(); // the function is aptly named
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