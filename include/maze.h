/* maze.h: Functions and definitions for maze pieces */
#ifndef __MAZE_H__
#define __MAZE_H__

/* Structure definitions, macros, and function prototypes go here */
#define WIDTH_MAZE 8
#define HEIGHT_MAZE 8


bool position_check(int x, int y);
bool left_path_check(int x, int y);
bool right_path_check(int x, int y);
bool top_path_check(int x, int y);
bool bottom_path_check(int x, int y);

#endif /* ! defined __MAZE_H__ */