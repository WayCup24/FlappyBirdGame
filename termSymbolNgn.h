#include <ncurses.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

struct Position {int x, y;};
typedef struct Position Point;

struct MatrixPack 
{
        int heigth;
        int width;
        char matrix[30][61];
};
typedef struct MatrixPack Matrix;

#define startButton 1
#define quitButton 2
#define Key_ENTER 10

void pointSum(Point *a, Point b)
{
        a->x += b.x;
        a->y += b.y;
}

bool kbhit()
{
        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting);
        return byteswaiting > 0;
}

void setPoint(Matrix *matrix, Point pos, char replaceTo)
{
        int x = pos.x;
        int y = pos.y;
        matrix->matrix[y][x] = replaceTo;
}

void copyPosition(Point from, Point *to)
{
        to->x = from.x;
        to->y = from.y;
}

void refreshMatrix(Matrix *matrix)
{
        matrix->heigth = 30;
        matrix->width = 61;
        clear();
        char *up_down = "############################################################";
        char *left_right = "#                                                          #";
        strcpy(matrix->matrix[0], up_down);
        strcpy(matrix->matrix[29], up_down);
        for(int i = 1; i < 29; i++)
        {
                strcpy(matrix->matrix[i], left_right);
        }
}

void printMatrix(Matrix matrix)
{
        for(int i = 0; i < 30; i++)
        {
                printw("%s\n", matrix.matrix[i]);
        }
        refresh();
}

int showMenu()
{
        char *strStart = "START \n";
        char *strStartS = ">START\n";
        char *strQuit = "QUIT  \n";
        char *strQuitS = ">QUIT \n";
        char menu[2][8];
        int select = 1;
        int key;
        while(1)
        {
                switch(select)
                {
                        case 1:
                                strcpy(menu[0], strStartS);
                                strcpy(menu[1], strQuit);
                                break;
                        case 2:
                                strcpy(menu[0], strStart);
                                strcpy(menu[1], strQuitS);
                                break;
                }
                clear();
                printw("***MENU***\n%s%s", menu[0], menu[1]);
                refresh();

                key = getch();
                if(key == KEY_DOWN) select++;
                else if(key == KEY_UP) select--;
                else if(key == 10) return select;
                if(select == 3) select = 1;
                else if(select == 0) select = 2;
        }
}

void showGameOver(int score)
{
        int key;
        while(1)
        {
                clear();
                printw("***GAME OVER***\nSCORE: %d\n", score);
                refresh();
                key = getch();
                if(key == 10) break;
        }
}

