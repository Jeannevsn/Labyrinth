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
    bool position_check(int x, int y) const;
    bool left_path_check(int x, int y) const;
    bool right_path_check(int x, int y) const;
    bool bottom_path_check(int x, int y) const;
    bool top_path_check(int x, int y) const;
    void print_state(void) const;
    void randomize_maze();

    bool is_finished(int x, int y) const;
    void get_start_position(int &x, int &y);
    void get_finish_position(int &x, int &y);
    void get_landmark_position(int &x_1, int &y_1, int &x_2, int &y_2);

private:
    int m_maze[9][8][8][2];
    int m_index_maze;
    int m_finish[2];
    int m_start[2];
    int m_landmark[9][2][2];
};

#endif /* ! defined __MAZE_H__ */