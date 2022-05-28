#pragma once
#include <string>
#include <iostream>
#include <wchar.h>
#include <atlimage.h>
#include "object.h"

#define BULL_SPRITES 8

class MyBackGround {
public:
	void static SetImage(LPCTSTR fileName);
	void Draw(HDC memdc, const RECT& rView);
private:
	static CImage img;
	static int imageW, imageH;
};


class MyObject {
public:
	MyObject() = default;
	MyObject(int x, int y, int w, int h);
protected:
	int x, y;
	int width, height;
};

class Bullbasaur : public MyObject {
public:
	Bullbasaur() = default;
	Bullbasaur(int x, int y, int width, int height);
	void static SetImage(LPCTSTR fileName);
	void Draw(HDC memdc);

private:
	static CImage img[BULL_SPRITES];
	static int imageW[BULL_SPRITES], imageH[BULL_SPRITES];
	int count;
};