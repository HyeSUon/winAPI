#include "object.h"



Bullbasaur::Bullbasaur(int x, int y, int w, int h) : x{ x }, y{ y }, width{ w }, height{ h }, frame{ 0 } { }
MyBackGround::MyBackGround(HWND hWnd) {
	GetClientRect(hWnd, &rectView);
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
	img[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW[frame], imageH[frame]);
	frame = ++frame % BULL_SPRITES;
}

void MyBackGround::SetImage(LPCTSTR fileName)
{
	img.Load(fileName);
	imageW = img.GetWidth();
	imageH = img.GetHeight();
}
void MyBackGround::Draw(HDC memdc)
{
	img.Draw(memdc, 0, 0, rectView.right, rectView.bottom, 0, 0, imageW, imageH);
}