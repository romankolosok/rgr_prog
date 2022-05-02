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

typedef struct{
    Vector2 recPos;
}center;

center Mas[4][4];//масив центрів клітинок


void drawBorders()
{
    Vector2 startPos, endPos;
    //DrawLineEx((Vector2){gapFromEdge,gapFromEdge-2},(Vector2){gapFromEdge,SCREEN_HEIGHT-gapFromEdge},5,BLACK);
    //DrawLineEx((Vector2){gapFromEdge-2,gapFromEdge},(Vector2){SCREEN_WIDTH-gapFromEdge,gapFromEdge},5,BLACK);
    //DrawRectangleLinesEx((Rectangle){gapFromEdge,gapFromEdge,SCREEN_WIDTH-2*gapFromEdge,SCREEN_HEIGHT-2*gapFromEdge},lineThick,BLACK);
    for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Mas[j][i].recPos.x = gapFromEdge+(sizeOfRect+lineThick)*i+sizeOfRect/2;
                Mas[j][i].recPos.y = gapFromEdge+(sizeOfRect+lineThick)*j+sizeOfRect/2;
                DrawRectangleLinesEx((Rectangle){gapFromEdge+(sizeOfRect+lineThick)*i,gapFromEdge+(sizeOfRect+lineThick)*j,sizeOfRect,sizeOfRect},lineThick,BLACK);
            }
    }
}

void shuffleInput(char **A)
{
    int rind;
    char *tmp;
    srand(time(0));
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tmp = A[4*i+j];
            rind = rand()%4;
            A[4*i+j]= A[4*i+rind];
            A[4*i+rind] = tmp;
        }
    }
}

void drawLetters(Font f,char **A)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            DrawTextEx(f,A[4*i+j],Mas[i][j].recPos,50,1,BLACK);
        }
    }
}

void test()
{
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                DrawCircleLines(Mas[j][i].recPos.x,Mas[j][i].recPos.y,5,BLACK);
            }
    }
}

Vector2 findClosest(Vector2 pos)
{
    Vector2 res = Mas[0][0].recPos;
    float min = sqrt(pow(res.x-pos.x,2)+pow(res.y-pos.y,2));
    float cur;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cur =  sqrt(pow(Mas[i][j].recPos.x - pos.x,2)+pow(Mas[i][j].recPos.y - pos.y,2));
            if(cur < min){
                min = cur;
                res = Mas[i][j].recPos;
            }
        }
    }
    return res;
}


int main()
{
    int start = 0;
    char *Inp[16] = {"a", "b", "c","d","a", "b", "c","d","a", "b", "c","d","a", "b", "c","d"};
    Vector2 cursorPos, closest;
    shuffleInput(Inp);
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "RGR #7");
    Font textF = LoadFont("resources/BluePrinted.ttf");
    SetTargetFPS(10);


    while(!WindowShouldClose()){
        BeginDrawing();
        if(drawRect){
            DrawRectangle(closest.x-sizeOfRect/2,closest.y-sizeOfRect/2,sizeOfRect,sizeOfRect,YELLOW);
        }
        ClearBackground(WHITE);
        drawBorders(Mas);
        drawLetters(textF,Inp);
        EndDrawing();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            //BeginDrawing();
            drawRect = true;
            cursorPos.x = GetMouseX();
            cursorPos.y = GetMouseY();
            closest = findClosest(cursorPos);
            //DrawRectangle(closest.x-sizeOfRect/2,closest.y-sizeOfRect/2,sizeOfRect,sizeOfRect,YELLOW);
            //EndDrawing();
        }

        //test();


    }
    return 0;
}
