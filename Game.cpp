#include <ncurses/ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>

struct Point {
    int x, y;
};

void spawnFood(Point &food, int widht, int height, const std::vector<Point>& snake){
    bool valid;
do {
    valid =true;
    food.x = rand() % (widht - 2) + 1;
    food.y = rand() % (height - 2) + 1;

    for(auto &s : snake){
        if(s.x == food.x && s.y == food.y){
        valid = false;
        break;
        }
    } 
}while (!valid);
}
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

    snake.clear();
    snake.push_back({width/2, height/2});

    dx = 1;
    dy = 0;

    spawnFood(food, width, height, snake);

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
// -----------------------------------------------------------------------
bool checkCollision(const std::vector<Point>& snake, int width, int height) {
    int x = snake[0].x;
    int y = snake[0].y;

    // Menabrak dinding
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1)
        return true;

    // Menabrak badan sendiri
    for (size_t i = 1; i < snake.size(); i++)
        if (snake[i].x == x && snake[i].y == y)
            return true;

    return false;
}

// -----------------------------------------------------------------------
//jalan ular
void moveSnake(std::vector<Point>& snake, int dx, int dy, bool grow) {
    Point newHead = {snake[0].x + dx, snake[0].y + dy};

    // Geser badan ular
    snake.insert(snake.begin(), newHead);

        if (!grow)
            snake.pop_back();
}
// -----------------------------------------------------------------------

int main() {
    WINDOW* win;
    int width, height;
    std::vector<Point> snake;
    Point food;
    int dx, dy;

    setupGame(win, height, width, snake, food, dx, dy);

    bool running = true;

    while (running) {
        int ch = getch();
        switch (ch) {
            case KEY_UP:    if (dy != 1)  { dx = 0; dy = -1; } break;
            case KEY_DOWN:  if (dy != -1) { dx = 0; dy = 1; }  break;
            case KEY_LEFT:  if (dx != 1)  { dx = -1; dy = 0; } break;
            case KEY_RIGHT: if (dx != -1) { dx = 1;  dy = 0; } break;
        }

        bool grow = false;

        if (snake[0].x == food.x && snake[0].y == food.y){
            grow = true;
            spawnFood(food, width, height, snake);
        }

        moveSnake(snake, dx, dy, grow);

        if (checkCollision(snake, width, height))
            running = false;

        draw(win, snake, food);
        napms(120);
    }

    mvwprintw(win, height/2, width / 2 - 5, "GAME OVER");
    wrefresh(win);
    nodelay(stdscr, FALSE);
    getch();

    endwin();
    return 0;
}


