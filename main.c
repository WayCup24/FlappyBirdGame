#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "termSymbolNgn.h"

#define playerTexture '@'

struct PipePack
{
        Point pipe, hole;
        int width, holeHight;
};
typedef struct PipePack Pipe;

Point player;
Pipe pipes[3];
Matrix matrix;

int g = 32;
int key, score;

void setPipe(Pipe pipe)
{
        bool visible, isHole;
        int holeDelta;
        for(int i = 0; i < pipe.width; i++)
        {
                visible = 0 <= pipe.pipe.x && pipe.pipe.x <= 59;
                if(visible)
                        for(int j = 0; j < 28; j++)
                        {
                                setPoint(&matrix, pipe.pipe, '#');
                                holeDelta = pipe.pipe.y - pipe.hole.y;
                                isHole = 0 <= holeDelta && holeDelta <= pipe.holeHight;
                                if(isHole) setPoint(&matrix, pipe.pipe, ' ');

                                pipe.pipe.y++;
                        }
                pipe.pipe.x++;
                pipe.pipe.y = 1;
        }
}

void refreshPipe(Pipe *pipe)
{
        if(pipe->pipe.x == 59)
        {
                srand(time(NULL));
                pipe->hole.y = rand() % 20 + 3;
        }

        if(pipe->pipe.x == 0) pipe->pipe.x = 59;
        else pipe->pipe.x--;

        setPipe(*pipe);

        if(pipe->pipe.x == player.x - 2) score++;
}

void setPlayerAngle(float velocity)
{
        Point playerBack;
        playerBack.x = player.x - 1;
        Point playerForward;
        playerForward.x = player.x + 1;
        if(velocity == 0)
        {
                playerBack.y = player.y;
                playerForward.y = player.y;
        }
        else if(velocity < 0)
        {
                playerBack.y = player.y + 1;
                playerForward.y = player.y - 1;
        }
        else if(velocity > 0)
        {
                playerBack.y = player.y - 1;
                playerForward.y = player.y + 1;
        }
        setPoint(&matrix, playerBack, playerTexture);
        setPoint(&matrix, playerForward, playerTexture);
}

void start()
{
        int currentPos;
        float velocity = 0;
        int sleepTime = 62500;
        float acceleration;
        int pipeX = 59;

        player.x = 15;
        player.y = 15;

        for(int i = 0; i < 3; i++)
        {
                pipes[i].pipe.x = pipeX;
                pipes[i].pipe.y = 1;
                pipes[i].width = 4;
                pipes[i].holeHight = 5;
                pipes[i].hole.x = pipes[i].pipe.x;
                pipeX += 20;
        }
        while(1)
        {
                refreshMatrix(&matrix);
                for(int i = 0; i < 3; i++)
                {
                        refreshPipe(&pipes[i]);
                }
                setPoint(&matrix, player, playerTexture);
                setPlayerAngle(velocity);
                printMatrix(matrix);

                acceleration = 0;
                currentPos = player.y;

                if(kbhit())
                {
                        key = getch();
                        if(key == KEY_UP)
                        {
                                acceleration = -288;
                                velocity = 0;
                        }
                }

                velocity = velocity + (acceleration + g) * sleepTime / 1000000;
                player.y = currentPos + round(velocity * sleepTime / 1000000);

                if(matrix.matrix[player.y][player.x] == '#')
                {
                        showGameOver(score);
                        score = 0;
                        break;
                }

                usleep(sleepTime);
        }
}

int main()
{
        initscr();
        keypad(stdscr, true);

        int selected;
        while(selected != 2)
        {
                selected = showMenu();
                if(selected == 1) start();
        }

        endwin();
        return 0;
}
