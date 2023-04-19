#include "maze.h"

int maze1[8][8][2] = {
    // Rangée 0
    {{0, 0}, {0, 1}, {0, 0}, {1, 0}, {0, 1}, {0, 1}, {1, 1}, {0, 1}},
    // Rangée 1
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 1}, {0, 1}, {0, 0}, {0, 1}},
    // Rangée 2
    {{0, 0}, {1, 1}, {0, 0}, {1, 0}, {0, 1}, {0, 0}, {0, 1}, {1, 1}},
    // Rangée 3
    {{0, 0}, {1, 1}, {0, 1}, {1, 1}, {1, 1}, {0, 0}, {1, 1}, {0, 1}},
    // Rangée 4
    {{0, 0}, {0, 1}, {0, 0}, {1, 0}, {0, 1}, {1, 0}, {0, 0}, {1, 1}},
    // Rangée 5
    {{0, 0}, {0, 0}, {1, 0}, {0, 0}, {1, 0}, {0, 0}, {1, 0}, {0, 0}},
    // Rangée 6
    {{1, 0}, {1, 1}, {1, 0}, {1, 1}, {0, 1}, {1, 1}, {1, 0}, {0, 0}},
    // Rangée 7
    {{1, 0}, {0, 0}, {1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, 0}, {1, 1}}};

bool position_check(int x, int y)
{
    // conditions for not leaving the matrix
    if (x < 0 || y < 0)
        return false;
    else
    {
        if (x > WIDTH_MAZE - 1 || y > 7)
            return false;
        else
            return true;
    }
}

bool left_path_check(int x, int y)
{
    if ((position_check(x - 1, y) == true) && (maze1[y][x][0] == 0))
        return true;
    else
        return false;
}

bool right_path_check(int x, int y)
{
    if ((position_check(x + 1, y) == true) && (maze1[y][x + 1][0] == 0))
        return true;
    else
        return false;
}

bool bottom_path_check(int x, int y)
{
    if ((position_check(x, y + 1) == true) && (maze1[y][x][1] == 0))
        return true;
    else
        return false;
}

bool top_path_check(int x, int y)
{
    if ((position_check(x, y - 1) == true) && (maze1[y - 1][x][1] == 0))
        return true;
    else
        return false;
}