#include "object.h"



MyObject::MyObject(int x, int y, int w, int h) : x{ x }, y{ y }, width{ w }, height{ h } { }
Bullbasaur::Bullbasaur(int x, int y, int w, int h) : MyObject(x, y, w, h), count{ 0 } {
}


void MyBackGround::SetImage(LPCTSTR fileName)
{
	img.Load(fileName);
	imageW = img.GetWidth();
	imageH = img.GetHeight();
}
void MyBackGround::Draw(HDC memdc, const RECT& rView)
{
	img.Draw(memdc, 0, 0, rView.right, rView.bottom, 0, 0, imageW, imageH);
}
void Bullbasaur::SetImage(LPCTSTR fileName)
{
	for (int i = 0; i < BULL_SPRITES; ++i) {
		TCHAR FullName[40];
		wsprintf(FullName, L"%s%d.png", fileName, i+1);
		img[i].Load(FullName);
		imageW[i]= img[i].GetWidth();
		imageH[i] = img[i].GetHeight();
	}

}

void Bullbasaur::Draw(HDC memdc)
{
	int halfW = width / 2;
	int halfH = height / 2;
	img[count].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW[count], imageH[count]);
	count = ++count % BULL_SPRITES;
}

