#pragma once
#include <windows.h>
#include <tchar.h>

#define OBJECT_SIZE 50 // ������Ʈ ����
#define BLOCK_MAX_SIZE 50 // BLOCK ���� ���� ��


// COUNT x COUNT �ٵ���
#define COUNT 40
// �� ���� ����
#define INTERVAL 20
#define WWIDTH COUNT*INTERVAL + 20 // ������ WIDTH ũ�� (20 ����ġ)
#define WHEIGHT (COUNT+2)*INTERVAL + 42 // ������ HEIGHT ũ�� (42 ����ġ)

// ���� ȭ��� ��ǥ
#define POS(x) ((x) * INTERVAL)
#define OBPOS(row, col) (col)* INTERVAL, (row)*INTERVAL, (col+1)*INTERVAL, (row+1)*INTERVAL

typedef enum Direction Direction;
typedef enum Pattern Pattern;

typedef struct Point Point;
typedef struct Object Object;
typedef struct Block Block;
typedef struct Player Player;
typedef struct ObjectManager ObjectManager;



void InitObject(ObjectManager* om);
void DrawObject(HDC hDC, const ObjectManager* om);
void PushObject(HWND hWnd, ObjectManager* om);
void MoveObject(ObjectManager* om);
void MovePlayer(ObjectManager* om, HWND hWnd);
void UpdateObject(ObjectManager* om);
void UpdatePlayer(ObjectManager* om);
void ClickUpdate(HWND hWnd, ObjectManager* om);
void PushBlock(ObjectManager* om);
void ResetGame(ObjectManager* om);
BOOL isObject(ObjectManager* om, int col, int row);
BOOL isBlock(ObjectManager* om, int col, int row);
void DeleteTail(ObjectManager* om, int i);
typedef enum Direction {
	LEFT, TOP, RIGHT, BOTTOM,
};
typedef enum Pattern {
	M1, M2, M3,
};

typedef struct Point {
	int col; int row;
};
typedef struct Object {
	int col; int row; Direction d; BOOL isUp; Point old; struct Object* childID; Pattern movePattern;
	int cnt; struct Object* parentID;
};
typedef struct Player {
	int col; int row; Direction d; BOOL isUp; Point old; struct Object* childID; unsigned int speed;
	COLORREF color; BOOL isRectangle;
};

typedef struct Block {
	int col; int row;
};

typedef struct ObjectManager {
	Player p;  Object obj[OBJECT_SIZE + 1]; int size; unsigned int objectMaxSize;
	int mCol; int mRow;// obj ������ ���� NULL�� (����x) 
	Block b[BLOCK_MAX_SIZE]; unsigned int blockCnt;
};