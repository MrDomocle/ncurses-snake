#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

int fieldSize[2] = {20, 40};
int snakeLength = 1;
int snake[100][2];
int headPos[2] = {1, 1};
int direction = 3; // 0 1 2 3 n s w e

const int MAXFRUIT = (fieldSize[0] * fieldSize[1]) / 100;
int fruitAm = 0;

void Input();
bool CollCheck(WINDOW *win, WINDOW *scorewin);
bool CollCheck(WINDOW *win, int pos[2]);
void GameLoop(WINDOW *win, WINDOW *scorewin);
void SeedFruit(WINDOW *win);

int main() {
    srand(time(nullptr));

    for (int i = 0; i < 100; i++) { snake[i][0] = 1; snake [i][1] = 1; }
    
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, true);
    
    WINDOW *gamewin = newwin(fieldSize[0] + 2, fieldSize[1] + 2, 0, 0);
    WINDOW *scorewin = newwin(3, fieldSize[1] + 2, fieldSize[0] + 1, 0);
    refresh();
    box(gamewin, 0, 0);
    box(scorewin, 0, 0);
    mvwaddstr(scorewin, 1, 1, "Score: 0");
    wrefresh(scorewin);
    GameLoop(gamewin, scorewin);

    nodelay(stdscr, false);
    curs_set(1);
    getch();
    endwin();
    return 0;
}

void GameLoop(WINDOW *win, WINDOW *scorewin) { // main game loop
    char fruitTick = 0;
    while (true) {
        Input();
        for (int r = 98; r >= 0; r--) { // shift snake
            snake[r + 1][0] = snake[r][0];
            snake[r + 1][1] = snake[r][1];
        }
        // check for collisions
        if (CollCheck(win, scorewin)) {
            break; // stop the game
        }
        // add headpos to the snake
        snake[0][0] = headPos[0];
        snake[0][1] = headPos[1];
        // draw the snake onto the screen
        int tailPos[2] = {snake[snakeLength][0], snake[snakeLength][1]};
        mvwaddch (win, tailPos[0], tailPos[1], ' '); // erase tail char
        mvwaddch(win, snake[0][0], snake[0][1], 's'); // add head char
        // Seed fruit every 20 frames
        if (fruitTick == 10) { 
            SeedFruit(win);
            fruitTick = -1;
        }
        fruitTick++;
        // finish up
        wrefresh(win);
        napms(150);
    }
}

bool CollCheck(WINDOW *win, WINDOW *scorewin) { // checks collision for the snake
    if ((mvwinch(win, headPos[0], headPos[1]) & A_CHARTEXT) != ' ' && (mvwinch(win, headPos[0], headPos[1]) & A_CHARTEXT) != '@') {
        mvwaddstr(scorewin, 1, 1, "Game over!");
        wrefresh(scorewin);
        return true;
    }
    if ((mvwinch(win, headPos[0], headPos[1]) & A_CHARTEXT) == '@') { // c o n s u m e
        if (snakeLength + 1 < 100) {
            snakeLength++;
            fruitAm--;
            string scorestr = "Score: " + std::to_string(snakeLength - 1);
            mvwaddstr(scorewin, 1, 1, scorestr.c_str());
            wrefresh(scorewin);
        }
        else {
            mvwaddstr(win, fieldSize[0] / 2, fieldSize[1] / 2 - 3, "YOU WON!");
            mvwaddstr(scorewin, 1, 1, ":expandDong:   press any key to exit");
            wrefresh(scorewin);
            wrefresh(win);
            nodelay(stdscr, false);
            getch();
            endwin();
            int *ptr = NULL;
            printf("%d", *ptr); // cause segfault because funny
            return true;
        }
    }
    return false;
}

bool CollCheck(WINDOW *win, int pos[2]) { // checks collision for a point(used by SeedFruit())
    if (mvwinch(win, pos[0], pos[1]) == ' ') {
        return false;
    }
    else {
        return true;
    }
}

void Input() { // get inputs and convert them to directions
    char input = getch();
    if (input) { // to not run switch if there hasn't been any input
        switch (input) {
            case 'w':
                if (direction > 1) {
                    direction = 0;
                } 
                break;
            case 's':
                if (direction > 1) {
                    direction = 1;
                }
                break;
            case 'a':
                if (direction < 2) {
                    direction = 2;
                }
                break;
            case 'd': 
                if (direction < 2) {
                    direction = 3;
                }
                break;
        }
    }
    switch (direction) { // set next head position
        case 0:
            if (headPos[0] - 1 >= 1) {
                headPos[0]--; // north
            }
            else {
                headPos[0] = fieldSize[0];
            }
            break;
        case 1:
            if (headPos[0] + 1 <= fieldSize[0]) {
                headPos[0]++; // south
            }
            else {
                headPos[0] = 1;
            }
            break;
        case 2:
            if (headPos[1] - 1 >= 1) {
                headPos[1]--; // west
            }
            else {
                headPos[1] = fieldSize[1];
            }
            break;
        case 3:
            if (headPos[1] + 1 <= fieldSize[1]) {
                headPos[1]++; // east
            }
            else {
                headPos[1] = 1;
            }
            break;
    }
}

void SeedFruit(WINDOW *win) { // seed fruit
    if (fruitAm < MAXFRUIT) {
        while (true) {
            int newFruitPos[2] = {(rand() + 1) % fieldSize[0], (rand() + 1) % fieldSize[1]};
            if (!CollCheck(win, newFruitPos)) { // place fruit and break the loop
                mvwaddch(win, newFruitPos[0], newFruitPos[1], '@');
                fruitAm++;
                break;
            }
        }
    }
}