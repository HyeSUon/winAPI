#pragma once
#include <string>
#include <iostream>
#include <wchar.h>
#include <atlimage.h>
#include "object.h"

#define BULL_SPRITES 8




class MyObject {
public:
	MyObject() = default;

	virtual void Draw(HDC memdc) = 0;

private:
};

class Bullbasaur : public MyObject {
public:
	Bullbasaur() = default;
	Bullbasaur(int x, int y, int width, int height);
	void static SetImage(LPCTSTR fileName);
	virtual void Draw(HDC memdc);

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
private:
	static CImage img;
	static int imageW, imageH;
	RECT rectView;
};