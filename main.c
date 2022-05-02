#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include <math.h>

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = SCREEN_HEIGHT;
const int gapFromEdge = 20;
const int lineThick = 3;
const int sizeOfRect = (SCREEN_HEIGHT-2*gapFromEdge)/4;
bool drawRect = false;

typedef struct
{
    Vector2 recPos;
    int ii;
    int jj;
} center;

center Mas[4][4];//масив центрів клітинок


void drawBorders()
{
    Vector2 startPos, endPos;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            Mas[j][i].recPos.x = gapFromEdge+(sizeOfRect+lineThick)*i+sizeOfRect/2;
            Mas[j][i].recPos.y = gapFromEdge+(sizeOfRect+lineThick)*j+sizeOfRect/2;
            Mas[j][i].ii = j;
            Mas[j][i].jj = i;
            DrawRectangleLinesEx((Rectangle)
            {
                gapFromEdge+(sizeOfRect+lineThick)*i,gapFromEdge+(sizeOfRect+lineThick)*j,sizeOfRect,sizeOfRect
            },lineThick,BLACK);
        }
    }
}

void shuffleInput(int A[4][4])
{
    int rind;
    int tmp;
    srand(time(0));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            tmp = A[i][j];
            rind = rand()%4;
            A[i][j]= A[i][rind];
            A[i][rind] = tmp;
        }
    }
}

void drawLetters(Font f,char *A[4], int B[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            DrawTextEx(f,A[B[i][j]-1],Mas[i][j].recPos,50,1,BLACK);
        }
    }
}


center findClosest(Vector2 pos)
{
    center res;
    res.recPos = Mas[0][0].recPos;
    float min = sqrt(pow(res.recPos.x-pos.x,2)+pow(res.recPos.y-pos.y,2));
    float cur;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cur =  sqrt(pow(Mas[i][j].recPos.x - pos.x,2)+pow(Mas[i][j].recPos.y - pos.y,2));
            if(cur < min)
            {
                min = cur;
                res.recPos = Mas[i][j].recPos;
                res.ii = Mas[i][j].ii;
                res.jj = Mas[i][j].jj;
            }
        }
    }
    return res;
}

bool isSolved(int A[4][4])
{
    int count = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            count = 0;
            for(int k = 0; k < 4; k++)
            {
                if(count < 2 && A[i][j] == A[i][k]) count++;
                if(count > 1) return false;
            }
        }
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            count = 0;
            for(int k = 0; k < 4; k++)
            {
                if(count < 2 && A[j][i] == A[k][i]) count++;
                if(count > 1) return false;
            }
        }
    }
    return true;
}


int main()
{
    int tmp;
    char *Inp[4] = {"a", "b", "c","d"};
    int IndexMas[4][4] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
    Vector2 cursorPos;
    center closest;
    closest.ii = 0;
    closest.jj = 0;
    shuffleInput(IndexMas);
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "RGR #7");
    Font textF = LoadFont("resources/BluePrinted.ttf");
    SetTargetFPS(10);


    while(!WindowShouldClose())
    {
        BeginDrawing();
        if(drawRect)
        {
            DrawRectangle(closest.recPos.x-sizeOfRect/2,closest.recPos.y-sizeOfRect/2,sizeOfRect,sizeOfRect,YELLOW);
        }
        ClearBackground(WHITE);
        drawBorders(Mas);
        drawLetters(textF,Inp,IndexMas);

        switch(GetKeyPressed())
        {
        case KEY_LEFT:
            if(closest.jj > 0 && drawRect)
            {
                tmp = IndexMas[closest.ii][closest.jj];
                IndexMas[closest.ii][closest.jj] = IndexMas[closest.ii][closest.jj-1];
                IndexMas[closest.ii][closest.jj-1] = tmp;
                drawRect = false;
            }
            break;
        case KEY_RIGHT:
            if(closest.jj < 3 && drawRect)
            {
                tmp = IndexMas[closest.ii][closest.jj];
                IndexMas[closest.ii][closest.jj] = IndexMas[closest.ii][closest.jj+1];
                IndexMas[closest.ii][closest.jj+1] = tmp;
                drawRect = false;
            }
            break;
        case KEY_UP:
            if(closest.ii > 0 && drawRect)
            {
                tmp = IndexMas[closest.ii][closest.jj];
                IndexMas[closest.ii][closest.jj] = IndexMas[closest.ii-1][closest.jj];
                IndexMas[closest.ii-1][closest.jj] = tmp;
                drawRect = false;
            }
            break;
        case KEY_DOWN:
            if(closest.ii < 3 && drawRect)
            {
                tmp = IndexMas[closest.ii][closest.jj];
                IndexMas[closest.ii][closest.jj] = IndexMas[closest.ii+1][closest.jj];
                IndexMas[closest.ii+1][closest.jj] = tmp;
                drawRect = false;
            }
            break;
        case KEY_ENTER:
            if(isSolved(IndexMas)) printf("SOLVED!!!\n");
            else printf("KEEP SOLVING...\n");
            break;
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            drawRect = true;
            cursorPos.x = GetMouseX();
            cursorPos.y = GetMouseY();
            closest = findClosest(cursorPos);
            if(closest.ii > 3 || closest.jj < 0)
            {
                closest.jj = 0;
                closest.ii = 0;
            }
        }
        EndDrawing();
    }
    return 0;
}
