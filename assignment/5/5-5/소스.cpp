#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <atlimage.h>
#include "object.h"

#define OBJECT_SIZE 10
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 0, WWIDTH, WHEIGHT+50, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc;
	static HBITMAP hBit;
	static MyObject* obj[OBJECT_SIZE];
	static Pokemon* player;
	static RECT rView;
	static int cntObj;
	TCHAR word[] = _T("대한민국 화이팅");
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rView);
		MyBackGround::SetImage(L".\\image\\bk.jpg", rView);
		Pokemon::SetImage(L".\\image\\P");
		MyBullet::SetImage(L".\\image\\star");
		obj[cntObj++] = new MyBackGround(hWnd);
		obj[cntObj++] = new Pokemon(300, 300, 30, 30, 10);

		for (int i = 0; i < cntObj; ++i)
			if (dynamic_cast<Pokemon*>(obj[i]))
				player = dynamic_cast<Pokemon*>(obj[i]);
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		player->Move();
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			if (cntObj < OBJECT_SIZE) {
				obj[cntObj] = new MyBullet(player->getX(), player->getY(), 20, 20, player->getD(), 30);
				++cntObj;
			}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBit = CreateCompatibleBitmap(hdc, rView.right, rView.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBit);
		::SetStretchBltMode(memdc, HALFTONE);
		for (int i = 0; i < cntObj; ++i) {
			if (dynamic_cast<MyBullet*>(obj[i])) {
				if (dynamic_cast<MyBullet*>(obj[i])->isRemove()) {
					delete obj[i];
					--cntObj;
					for (int j = i; j < cntObj; ++j) {
						obj[j] = obj[j + 1];
					}					
					--i; // 삭제되면서 한칸씩 당겼기 때문
				}
			}
			#pragma warning(suppress: 6385)
			if(i < OBJECT_SIZE)
				obj[i]->Update();
		}
		for (int i = 0; i < cntObj; ++i)
			obj[i]->Draw(memdc);
		BitBlt(hdc, 0, 0, rView.right, rView.bottom, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
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