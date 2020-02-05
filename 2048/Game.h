#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define CELL_W 9
#define CELL_H 5
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

bool checkGameEnd();
void drawBoard();
bool swipe(int);
void appendElement();
void showMenu();
void initGame();
void drawCell(int, int);
void goToXY(int, int);
bool order(int*);