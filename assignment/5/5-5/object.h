#pragma once
#include <string>
#include <iostream>
#include <wchar.h>
#include <atlimage.h>
#include "object.h"

// COUNT x COUNT 바둑판
#define COUNT 20
// 줄 사이 간격
#define INTERVAL 30
#define WWIDTH COUNT*INTERVAL + 20 // 윈도우 WIDTH 크기 (20 가중치)
#define WHEIGHT COUNT*INTERVAL + 42 // 윈도우 HEIGHT 크기 (42 가중치)
// 실제 화면상 좌표
#define POS(x) ((x) * INTERVAL)
#define OBPOS(row, col) (col)* INTERVAL, (row)*INTERVAL, (col+1)*INTERVAL, (row+1)*INTERVAL
// SPRITE COUNT
#define POKEMON_SPRITES 24
#define BULLET_SPRITES 4

enum D {
	B,
	LB,
	L,
	LT,
	T,
	RT,
	R,
	RB,
};


class MyObject {
public:
	MyObject() = default;

	virtual void Draw(HDC memdc) = 0;
	virtual void Update() { }
private:
};

class MyBackGround : public MyObject {
public:
	MyBackGround(HWND hWnd);
	void static SetImage(LPCTSTR fileName, const RECT& rectView);
	virtual void Draw(HDC memdc);
	virtual void Update();
	static RECT GetRectView() {
		return rectView;
	}


private:
	static RECT rectView;
	static CImage img;
	static int imageW, imageH;
};

class MyBullet : public MyObject {
public:
	MyBullet() = default;
	MyBullet(int x, int y, int width, int height, D d, int speed);
	void static SetImage(LPCTSTR fileName);
	virtual void Draw(HDC memdc);
	virtual void Update();
	bool isRemove();
	static CImage img[BULLET_SPRITES];
	static int imageW[BULLET_SPRITES], imageH[BULLET_SPRITES];
private:
	int x, y, width, height;
	D d;  // 8 direc
	int frame;
	int speed;
};

class Pokemon : public MyObject {
public:
	Pokemon() = default;
	Pokemon(int x, int y, int width, int height, int speed);
	void static SetImage(LPCTSTR fileName);
	virtual void Update();
	virtual void Draw(HDC memdc);
	void Move();

	D getD();
	int getX();
	int getY();
	
	static CImage img[POKEMON_SPRITES];
	static int imageW[POKEMON_SPRITES], imageH[POKEMON_SPRITES];
private:
	int x, y, width, height;
	D d;  // 8 direc
	int frame;
	int speed;
};