#include <ncurses.h>

int maxRows = 10;
int maxCols = 20;
int snakeLength = 3;
bool cDir = false; // left2right
bool rDir = false; // top2bottom

void GameLoop() {
    for (int r = 0; r < maxRows; r++) {
        for (int c = 0; c <= maxCols; c++) {
            int col = (cDir) ? -1*(c - maxCols) : c;
            int row = (rDir) ? -1*(r - maxRows) : r;
            mvaddch(row, col, 'h');
            // Clean older chars to stop the snake from looking stupid
            if (c >= snakeLength) { // if the whole snake + 1 char fits on a single row
                int tailCol = (cDir) ? col + snakeLength : col - snakeLength;
                mvaddch(row, tailCol, ' ');
            }
            else if (r != 0) { // if the snake is going between two lines and not first line
                int tailRow = (rDir) ? row + 1 : row - 1;
                int tailCol = (cDir) ? maxCols - snakeLength + c + 1: snakeLength - c - 1;
                mvaddch(tailRow, tailCol, ' ');
            }

            getch();

            refresh();
        }
        cDir = !cDir;
    }
    return;
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    GameLoop();
    getch();
    endwin();
    return 0;
}