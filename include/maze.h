/* maze.h: Functions and definitions for maze pieces */
#ifndef __MAZE_H__
#define __MAZE_H__

/* Structure definitions, macros, and function prototypes go here */
#define WIDTH_MAZE 8
#define HEIGHT_MAZE 8

class Maze
{
public:
    Maze();
    bool position_check(int x, int y);
    bool left_path_check(int x, int y, int game_maze[8][8][2]);
    bool right_path_check(int x, int y, int game_maze[8][8][2]);
    bool bottom_path_check(int x, int y, int game_maze[8][8][2]);
    bool top_path_check(int x, int y, int game_maze[8][8][2]);
    int random_maze(void);
    void print_state(void);

private:
    int m_maze1[8][8][2];
    int m_maze2[8][8][2];
    int m_maze3[8][8][2];
    int m_maze4[8][8][2];
    int m_maze5[8][8][2];
    int m_maze6[8][8][2];
    int m_maze7[8][8][2];
    int m_maze8[8][8][2];
    int m_maze9[8][8][2];
    int m_finish_maze[9][2];
};

#endif /* ! defined __MAZE_H__ */