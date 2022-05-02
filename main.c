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
bool previewScr = true;
bool gameInit = false;
bool tipsInit  = false;
bool gameEnd = false;

typedef struct
{
    Vector2 recPos;
    int ii;
    int jj;
} center; //структура для запамятовування кординат кожного квадрата і його відповідності елементу в масиві

center Mas[4][4];//масив центрів клітинок


void drawBorders()//процедура малювання "сітки" з квадратів
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //записуємо кординати центра квадрата
            Mas[j][i].recPos.x = gapFromEdge+(sizeOfRect+lineThick)*i+sizeOfRect/2;
            Mas[j][i].recPos.y = gapFromEdge+(sizeOfRect+lineThick)*j+sizeOfRect/2;
            //записуємо відповідність цього квадрата елементу масива
            Mas[j][i].ii = j;
            Mas[j][i].jj = i;
            //малюємо квадрат
            DrawRectangleLinesEx((Rectangle){gapFromEdge+(sizeOfRect+lineThick)*i,gapFromEdge+(sizeOfRect+lineThick)*j,sizeOfRect,sizeOfRect},lineThick,BLACK);
        }
    }
}

void shuffleInput(int A[4][4])//процедура перемішування "букв"
//використовується масив з числами бо з ними трохи легше оперувати
//відповідному індексу відповідає число з масива з чотирма буквами-рядками
//Масив букв-рядків використовується тому що функція друку тексту приймає тільки аргументи типу char*
{
    int rind;
    int tmp;
    srand(time(0));
    //знаходимо випадковий індекс в цей момент часу і змінюємо його елемент з елементом початкового індекса
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

void drawLetters(Font f,char *A[4], int B[4][4])//процедура малювання букв
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            DrawTextEx(f,A[B[i][j]-1],Mas[i][j].recPos,50,1,BLACK);
        }
    }
}


center findClosest(Vector2 pos)//процедура знаходження найближчого квадрата
//при кліку мишкою на екран потрібно вибрати найближчий квадрат, значення якого буде зсунуте
{
    center res;
    res.recPos = Mas[0][0].recPos;
    //шукаємо мінімальну відстань від позиції переданої в функцію до позиції наявної в масиві центрів квадратів
    float min = sqrt(pow(res.recPos.x-pos.x,2)+pow(res.recPos.y-pos.y,2));
    float cur;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cur =  sqrt(pow(Mas[i][j].recPos.x - pos.x,2)+pow(Mas[i][j].recPos.y - pos.y,2));
            //використовуючи формули відстані між 2 точками на площині знаходимо мінімум
            if(cur < min)
            {
                min = cur;
                res.recPos = Mas[i][j].recPos;
                res.ii = Mas[i][j].ii;
                res.jj = Mas[i][j].jj;
            }//знайшовши мінімум присвоюємо обєкту центр відповідні параматри найближчого квадрата
        }
    }
    return res;
}

bool isSolved(int A[4][4])//процедура перевірки вирішеності поставленої задачі
{
    int count = 0;
    //проходимося по рядках шукаючи дублікати
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            count = 0;
            for(int k = 0; k < 4; k++)
            {
                if(count < 2 && A[i][j] == A[i][k]) count++;
                //якщо знайшли дублікати то одразу повертаємо false
                if(count > 1) return false;
            }
        }
    }
    //якщо в рядках дублікатів немає, то шукаємо їх в стовпцях
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
    //якщо ні в рядках ні в стовпцях немає дублікатів повертаємо true
    return true;
}


int main()
{
    int tmp;
    char *Inp[4] = {"a", "b", "c","d"};//масив букв-рядків
    int IndexMas[4][4] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};//початковий масив чисел
    Vector2 cursorPos;
    center closest;
    shuffleInput(IndexMas);// перемішуємо початковий масив по рядкам
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "RGR #7");
    Font textF = LoadFont("resources/BluePrinted.ttf");//імпортую шрифт
    Font prevF = LoadFont("resources/Lobster-Regular.ttf");
    Texture2D mouse = LoadTexture("resources/mouse.png");
    Texture2D arrows = LoadTexture("resources/arrows.png");
    SetTargetFPS(10);


    while(!WindowShouldClose())//цикл графічної програми, який працює доки не натиснуто хрестик або ESC
    {
        BeginDrawing();
        if(gameInit){
            if(drawRect)
            {
                DrawRectangle(closest.recPos.x-sizeOfRect/2,closest.recPos.y-sizeOfRect/2,sizeOfRect,sizeOfRect,YELLOW);//виділення відповідно обраного квадрата
            }
            ClearBackground(WHITE);
            drawBorders(Mas);//малюємо сітку
            drawLetters(textF,Inp,IndexMas);//малюємо букви

            switch(GetKeyPressed())//оператор вибору, який відповідно від натиснутої клавіші пересуває вибрані елементи
            //елементи можуть пересовуватись на існуючі позиції в масиві, якщо спробувати пересунути елемент з правого крайнього стовпчика ВПРАВО, то нічого не відбудеться
            //аналогічно і для інших крайніх положень у масиві
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
                    if(isSolved(IndexMas)){
                        gameInit = false;
                        gameEnd = true;
                    }
                    else ClearBackground(RED);
                    break;
                case KEY_T:
                    gameInit = false;
                    tipsInit = true;
                    break;
            }

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))//при натисненні ЛКМ отримуємо її позицію
            {
                drawRect = true;
                cursorPos.x = GetMouseX();
                cursorPos.y = GetMouseY();
                //шукаємо найближчий квадрат
                closest = findClosest(cursorPos);
                if(closest.ii > 3 || closest.jj < 0)//це НЕОБХІДНА перевірка, якщо її не зробити програма працює некоректно у випадку пересування елемента на позиції [0][0]
                //там зявляються якісь великі і незрозумілі значення тому щоб уникути крашу програми виконується ця перевірка
                //після цієї перевірки все працює коректно
                {
                    closest.jj = 0;
                    closest.ii = 0;
                }
            }
        }else{//вивід різних вікон
            if(gameEnd){
                ClearBackground(WHITE);
                DrawTextEx(prevF,"CONGRATULATIONS!\n   YOU WON!\n\n\nto exit press ESC or click on CROSS",(Vector2){200,300},40,1,BLACK);
            }
            if(tipsInit){
                ClearBackground(WHITE);
                previewScr = false;
                ClearBackground(WHITE);
                DrawTextureEx(mouse,(Vector2){650,50},0,0.25,WHITE);
                DrawTextureEx(arrows,(Vector2){500,210},0,0.25,WHITE);
                DrawTextEx(prevF,"TIPS:\n1.Choose any square by clicking on it LMB\n\n2.Using arrows on keyboard\n move element to nearby square\n(*if \
element is on edge position it won't move to direction\nout of character array)\n\n3.If you want to check if you put letters in correct order\n press ENTER\
\n\nif you want to return to TIPS press T on keyboard",(Vector2){50,50},40,1,BLACK);
                if(KEY_ENTER == GetKeyPressed()){
                        gameInit = true;
                        tipsInit = false;
                }
            }
            if(previewScr){
                ClearBackground(WHITE);
                DrawTextEx(prevF,"RGR Variant #7 \nStudent group KV-14\nKolosok Roman",(Vector2){100,50},40,1,BLACK);
                DrawTextEx(prevF,"About:\n1.Given area 4x4 squares size \n2.You should put letters in order when each letter \nrepeats in each row and column ONCE",(Vector2){100,300},40,1,BLACK);
                DrawTextEx(prevF,"press ENTER to continue...",(Vector2){100,700},40,1,BLACK);
                if(KEY_ENTER == GetKeyPressed()){
                        previewScr = false;
                        tipsInit = true;
                }
            }
        }
        EndDrawing();
    }
    return 0;
}
