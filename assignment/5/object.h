#pragma once
#include <string>
#include <iostream>
#include <wchar.h>
#include <atlimage.h>
#include "object.h"

#define BULL_SPRITES 8
#define POKEMON_SPRITES 24

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
	virtual void Update() = 0;
private:
};

class Bullbasaur : public MyObject {
public:
	Bullbasaur() = default;
	Bullbasaur(int x, int y, int width, int height);
	void static SetImage(LPCTSTR fileName);
	virtual void Draw(HDC memdc);
	virtual void Update();
private:
	static CImage img[BULL_SPRITES];
	static int imageW[BULL_SPRITES], imageH[BULL_SPRITES];
	int x, y, width, height;
	int frame;
};

class MyBackGround : public MyObject {
public:
	MyBackGround(HWND hWnd);
	void static SetImage(LPCTSTR fileName);
	virtual void Draw(HDC memdc);
	virtual void Update();
private:
	static CImage img;
	static int imageW, imageH;
	RECT rectView;
};

class Pokemon : public MyObject {
public:
	Pokemon() = default;
	Pokemon(int x, int y, int width, int height, int speed);
	void static SetImage(LPCTSTR fileName, LPCTSTR fileName2, LPCTSTR fileName3);
	virtual void Update();
	virtual void Draw(HDC memdc);
	void Move(const RECT& rView);
	void KeyEvent(WPARAM wParam);
	void MouseEvent(int mx, int my);

	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getFrame();
	D getD();
	TCHAR getCharacter();
	static CImage img[POKEMON_SPRITES];
	static int imageW[POKEMON_SPRITES], imageH[POKEMON_SPRITES];

	static CImage img2[POKEMON_SPRITES];
	static int imageW2[POKEMON_SPRITES], imageH2[POKEMON_SPRITES];

	static CImage img3[POKEMON_SPRITES];
	static int imageW3[POKEMON_SPRITES], imageH3[POKEMON_SPRITES];
private:
	int x, y, width, height;
	D d;  // 8πÊ«‚
	int frame;
	int speed;
	TCHAR character;
	bool isJumping, isBig, isSmall, isBallMode, isTransform;
	int tempX, tempY;
};



class SpriteObject : public MyObject {
public:
	SpriteObject() = default;
	SpriteObject(int x, int y, int width, int height, int frame, TCHAR c);
	virtual void Update();
	virtual void Draw(HDC memdc);
private:
	int x, y, width, height, frame;
	TCHAR character;
};
