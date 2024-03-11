// library include
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif
#include "bouton.h"
#include "maze.h"

#define PIN 26 // pin matrix initialization
#define PIN2 12

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // MAC address of the receiver

typedef struct struct_message
{
  char a[32];
  int erreur;
  int timer;
  int timer_value;
  int module;
  int difficulty;
  int level[2];
  int condo;
  int num_serie;
  bool init_condo;
} struc_message;

Bouton bt[4];                                              // buttons initialization
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN); // matrix initialization
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(256, PIN2, NEO_GRB + NEO_KHZ800);
uint16_t grey = matrix.Color(40, 40, 40);                  // initialization of a "new" color
struct_message myData;

esp_now_peer_info_t peerInfo; // peer info initialization

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) // function to check if the message has been sent
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) // function to check if the message has been received
{
  memcpy(&myData, data, sizeof(myData));
}

const int FDC = 18;
const int Valcondo = 14;
const int charge = 26;

// initialization game settings
int i = 0, j = 0;
int counter = 0;
int game_state = 0;
int number_errors = 0;
int x, y, x_finish, y_finish, x_start, y_start, x_1, y_1, x_2, y_2;

const int bt_blue = 4, bt_yellow = 17, bt_black = 16, bt_green = 13;          // test buttons initiation
int button_table[4] = {bt_blue, bt_yellow, bt_green, bt_black};               // set test buttons table

void setup_bt(int nb_bt);                                                     // initialization of the buttons
void read_bt(int nb_bt);                                                      // reading of the buttons

int valeur_condo = 0;

Maze maze_game;

void setup()
{
  Serial.begin(115200);
  // matrix initialization
  matrix.clear();
  matrix.begin();
  matrix.setTextWrap(false);
  // pins initiation
  pinMode(bt_yellow, INPUT);
  pinMode(bt_green, INPUT);
  pinMode(bt_black, INPUT);
  pinMode(bt_blue, INPUT);
  pinMode(FDC, INPUT);
  pinMode(Valcondo, INPUT);

  // turn off the devil led
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(charge, OUTPUT);
   
  // fill the matrix full grey
  setup_bt(4);
  for (i = 0; i <= 7; i++)
  {
    for (j = 0; j <= 7; j++)
    {
      matrix.drawPixel(i, j, grey);
    }
  }
  
  maze_game.get_start_position(x, y);
  maze_game.get_finish_position(x_finish, y_finish);
  maze_game.get_landmark_position(x_1, y_1, x_2, y_2);
  matrix.setBrightness(150); // set the next LEDs in the matrix to 150 brightness
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

  matrix.drawPixel(x_1, y_1, MAGENTA);
  matrix.drawPixel(x_2, y_2, MAGENTA);

  switch (game_state)
  {
  case 0: // game initialization
    matrix.drawPixel(x, y, BLUE);
    matrix.drawPixel(x_finish, y_finish, RED);

    game_state = 1;
    break;

  case 1:
    // conditions for changing the led position
    Serial.printf("\r (%d,%d)  (%d, %d, %d, %d)", x, y, maze_game.left_path_check(x, y), maze_game.top_path_check(x, y), maze_game.bottom_path_check(x, y), maze_game.right_path_check(x, y));
    maze_game.print_state();
    if (bt[0].click() == HIGH)
    {
      {
        if (maze_game.left_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          x--;
        }
        else
        {
          if (number_errors < 2)
            game_state = 4;
          else if (number_errors > 2)
            game_state = 3;
        }
      }
    }
    else if (bt[1].click() == HIGH)
    {
      {
        if (maze_game.right_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          x++;
        }
        else
        {
          if (number_errors < 2)
            game_state = 4;
          else if (number_errors > 2)
            game_state = 3;
        }
      }
    }
    else if (bt[2].click() == HIGH)
    {
      {
        if (maze_game.bottom_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          y++;
        }
        else
        {
          if (number_errors < 2)
            game_state = 4;
          else if (number_errors > 2)
            game_state = 3;
        }
      }
    }
    else if (bt[3].click() == HIGH)
    {
      {
        if (maze_game.top_path_check(x, y))
        {
          matrix.drawPixel(x, y, grey);
          y--;
        }
        else
        {
          if (number_errors < 2)
            game_state = 4;
          else if (number_errors > 2)
            game_state = 3;
        }
      }
    }

    // victory condition
    if (maze_game.is_finished(x, y))
    {
      game_state = 2;
    }
    matrix.show(); // the function is aptly named
    break;

  case 2: // win
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, GREEN);
      }
    }
    matrix.show();
    break;

  case 3: // lost with 3 errors
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, RED);
      }
    }
    matrix.show();
    break;

  case 4: // lost with one errors
    number_errors++;
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, WHITE);
      }
    }
    matrix.show();
    delay(1000);
    maze_game.randomize_maze();
    maze_game.get_start_position(x, y);
    maze_game.get_finish_position(x_finish, y_finish);
    maze_game.get_landmark_position(x_1, y_1, x_2, y_2);
    game_state = 0;
    for (i = 0; i <= 7; i++)
    {
      for (j = 0; j <= 7; j++)
      {
        matrix.drawPixel(i, j, grey);
      }
    }
    break;

  default:
    game_state = 0;
  }
  if (game_state != 2 || game_state != 3)
  {
    matrix.drawPixel(x, y, BLUE);
  }

  int valeur = analogRead(valeur_condo);
  for(int i = 0; i < map(valeur, 0, 1023, 0, 256); i++){
      pixels.setPixelColor(i, pixels.Color(map(valeur, 0, 1023, 255, 0), 0, map(valeur, 0, 1023, 0, 255)));
    }
  if (digitalRead(FDC) == HIGH){
    digitalWrite(charge, HIGH);
  }else{
    digitalWrite(charge, LOW);
  }
}

void setup_bt(int nb_bt)
{
  /*
    Input : nb_bt : number of buttons (int)
    Output : none
    Description: This function is used to initialize the buttonsa
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