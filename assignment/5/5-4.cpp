#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <atlimage.h>
#include "object.h"
HINSTANCE hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

CImage Bullbasaur::img[BULL_SPRITES], MyBackGround::img, Pikachu::img[PIKA_SPRITES], Pikachu::img2[PIKA_SPRITES], Pikachu::img3[PIKA_SPRITES];
int Bullbasaur::imageW[BULL_SPRITES], Bullbasaur::imageH[BULL_SPRITES];
int Pikachu::imageW[PIKA_SPRITES], Pikachu::imageH[PIKA_SPRITES], Pikachu::imageW2[PIKA_SPRITES], Pikachu::imageH2[PIKA_SPRITES];
int Pikachu::imageW3[PIKA_SPRITES], Pikachu::imageH3[PIKA_SPRITES];


int MyBackGround::imageW, MyBackGround::imageH;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc;
	static HBITMAP hBit;
	static MyObject* obj[20];
	static Pikachu* pika;
	static int cntObj;
	static RECT rectView;

	static int mx, my;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);

		MyBackGround::SetImage(L".\\image\\bg.jpg");
		Bullbasaur::SetImage(L".\\image\\O");
		Pikachu::SetImage(L".\\image\\P", L".\\image\\E", L".\\image\\F");
		obj[cntObj++] = new MyBackGround(hWnd);
		obj[cntObj++] = new Pikachu(200, 200, 30, 50, 10);

		// 조작캐릭터 찾기 (여러 마리면 마지막 객체를 가리킴)
		for (int i = 0; i < cntObj; ++i)
			if (dynamic_cast<Pikachu*>(obj[i]))
				pika = dynamic_cast<Pikachu*>(obj[i]);
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		pika->Move(rectView);
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rectView);
		hdc = BeginPaint(hWnd, &ps);
		hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBit);
		::SetStretchBltMode(memdc, HALFTONE);
		for (int i = 0; i < cntObj; ++i) {
			obj[i]->Update();
			obj[i]->Draw(memdc);
		}
		BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		pika->KeyEvent(wParam);
		if (wParam == 't') {
			static int cntSprite = 0;
			if (cntSprite < 5) {
				obj[cntObj++] = new SpriteObject(pika->getX(), pika->getY(), pika->getWidth(), pika->getHeight(),
					pika->getFrame(), pika->getCharacter());
				++cntSprite;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		pika->MouseEvent(mx, my);
		break;
	case WM_DESTROY:
		if (hBit) DeleteObject(hBit);
		for (int i = 0; i < cntObj; ++i)
			delete obj[i];
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}