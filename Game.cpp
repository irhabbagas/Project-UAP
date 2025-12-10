#include <ncurses/ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>

struct Point {
    int x, y;
};

void setupGame(WINDOW*& win, int& height, int& width, 
               std::vector<Point>& snake, Point& food, 
               int& dx, int& dy) 
{
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(0));

    height = 20;
    width = 40;

    win = newwin(height, width, 1, 1);
    box(win, 0, 0);
    wrefresh(win);

    snake = {{width/2, height/2}};
    dx = 1;
    dy = 0;

    food = {rand() % (width - 2) + 1, rand() % (height - 2) + 1};
}
// -----------------------------------------------------------------------

void draw(WINDOW* win, const std::vector<Point>& snake, const Point& food) {
    werase(win);
    box(win, 0, 0);

    // Draw food
    mvwprintw(win, food.y, food.x, "O");

    // Draw snake
    for (size_t i = 0; i < snake.size(); i++) {
        mvwprintw(win, snake[i].y, snake[i].x, i == 0 ? "@" : "*");
    }

    wrefresh(win);
}

