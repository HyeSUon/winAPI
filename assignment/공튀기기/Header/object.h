#pragma once
#include <stdio.h>
#include <math.h>
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>

#define BLOCK_MAX_LINE 5
#define BLOCK_MAX_COUNT 10
#define PI 3.1415926535897




typedef enum Collide Collide;

typedef struct Line Line; // 선으로 충돌 판정
typedef struct Ball Ball;
typedef struct Block Block;
typedef struct Stick Stick;
typedef struct ObjectManager ObjectManager;


void InitObject(ObjectManager* om);

void MoveBall(Ball* ball);
void MoveStick(WPARAM wParam, Stick* stick);
void MoveBlocks(Block[BLOCK_MAX_LINE][BLOCK_MAX_COUNT], BOOL* isLeft, RECT cr);

void DrawBall(HDC hDC, const Ball* ball);
void DrawStick(HDC hDC, const Stick* stick);
void DrawROPStick(HDC hDC,LPARAM lParam, Stick* stick);
void DrawBlocks(HDC hDC, const Block[BLOCK_MAX_LINE][BLOCK_MAX_COUNT]);

void InFingerSnap(Ball* ball, const RECT* r, BOOL isLeft, BOOL isTop, BOOL isRight, BOOL isBottom);
BOOL OutFingerSnap(Ball* ball, const RECT* r, BOOL isLeft, BOOL isTop, BOOL isRight, BOOL isBottom);
BOOL CollideCirRec(RECT* r, Ball* ball);
void CollideBlock(Block[][BLOCK_MAX_COUNT], const Ball* ball, int* broken, int* smallCnt);

BOOL FallBall(Ball* ball, const Stick* stick, RECT cr);
BOOL InRectangle(RECT r, int mx, int my);
BOOL IsPointBall(Ball* b, int x, int y);
enum Collide {
	LEFT, TOP, RIGHT, BOTTOM,
};


struct Ball {
	double x; double y; // 원의 좌표
	double r;
	double a; // 가속도
	double theta; // 원의 방향
	COLORREF c;
};

struct Block {
	RECT r; BOOL isSmall;
};
struct Stick {
	RECT r; RECT oldR; int size;
	int speed;
};

struct ObjectManager {
	Block block[BLOCK_MAX_LINE][BLOCK_MAX_COUNT];
	BOOL isLeftBlock;
	int blockSize; int blockMaxSize; int blockMaxLine; RECT rClient; int brokenBlock; int smallBlock;
	Ball ball;
	Stick stick;
};