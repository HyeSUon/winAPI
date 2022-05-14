#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include "object.h"
#include "resource.h"

#define OBJECT_TIMER 300
#define OBJECT_SPEED 200

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"꼬리원 게임";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 0, WWIDTH, WHEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}






void DrawBoard(HDC hDC);


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static ObjectManager om;
	static int cntU, cntJ;
	static BOOL isUFail;
	static BOOL running;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		InitObject(&om);
		SetTimer(hWnd, 2, om.p.speed, NULL);
		SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
		SetTimer(hWnd, 3, OBJECT_SPEED, NULL);
		break;
	case WM_TIMER:
		if (running) {
			switch (wParam) {
			case 1:
				PushObject(hWnd, &om);
				break;
			case 2:
				MovePlayer(hWnd, &om);
				UpdatePlayer(&om);
				break;
			case 3:
				MoveObject(&om);
				UpdateObject(&om);
				break;
			case 4:
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 2, om.p.speed, NULL);
				break;
			case 5: // U turn
				if (cntU == 2)
					cntU = 0;
				if (isUFail) { // 사이드에서 block을 만날 시 error
					om.p.d = (om.p.d - 1);
					if (om.p.d < 0)
						om.p.d = 3;
				}
				else {
					om.p.d = (om.p.d + 1) % 4;
				}

				if (++cntU == 2) {
					isUFail = !isUFail;
					KillTimer(hWnd, 5);
				}
				break;
			case 6:
				if (cntJ == 4)
					cntJ = 0;
				if (cntJ == 0 || cntJ == 3)
					om.p.d = (om.p.d + 1) % 4;
				else {
					om.p.d = (om.p.d - 1);
					if (om.p.d < 0)
						om.p.d = 3;
				}
				if (++cntJ == 4)
					KillTimer(hWnd, 6);
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		if (running) {
			om.mCol = LOWORD(lParam) / INTERVAL;
			om.mRow = HIWORD(lParam) / INTERVAL;

			ClickUpdate(hWnd, &om);
		}
		break;
	case WM_RBUTTONDOWN:
		om.mCol = LOWORD(lParam) / INTERVAL;
		om.mRow = HIWORD(lParam) / INTERVAL;
		PushBlock(&om);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		DrawBoard(hDC);
		DrawObject(hDC, &om);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		switch (wParam) {
		case 's':
			running = TRUE;
			break;
		case 'p':
			running = !running;
			break;
		case 'q':
			PostQuitMessage(0);
			break;
		case 'r':
			ResetGame(&om);
			InitObject(&om);
			SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
			running = FALSE;
			break;
		case '+':
			if (om.p.speed > 4)
				om.p.speed -= 4;
			SetTimer(hWnd, 2, om.p.speed, NULL);
			break;
		case '-':
			om.p.speed += 4;
			SetTimer(hWnd, 2, om.p.speed, NULL);
			break;
		case 'j':
			SetTimer(hWnd, 6, om.p.speed, NULL);
			break;
		case 't':
			SetTimer(hWnd, 5, om.p.speed, NULL);
			break;
		}
		break;
	case WM_KEYDOWN:
		if (running) {
			switch (wParam) {
			case VK_LEFT:
				om.p.d = LEFT;
				break;
			case VK_RIGHT:
				om.p.d = RIGHT;
				break;
			case VK_UP:
				om.p.d = TOP;
				break;
			case VK_DOWN:
				om.p.d = BOTTOM;
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_GAME_START:
			running = TRUE;
			break;
		case ID_GAME_RESET:
			running = FALSE;
			ResetGame(&om);
			InitObject(&om);
			SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_GAME_END:
			PostQuitMessage(0);
			break;
		case ID_SPEED_FAST:
			om.p.speed = 100;
			SetTimer(hWnd, 2, 100, NULL);
			SetTimer(hWnd, 3, 100, NULL);
			break;
		case ID_SPEED_MEDIUM:
			om.p.speed = 150;
			SetTimer(hWnd, 2, 150, NULL);
			SetTimer(hWnd, 3, 150, NULL);
			break;
		case ID_SPEED_SLOW:
			om.p.speed = 300;
			SetTimer(hWnd, 2, 300, NULL);
			SetTimer(hWnd, 3, 300, NULL);
			break;
		case ID_COLOR_CYAN:
			om.p.color = RGB(0, 255, 255);
			break;
		case ID_COLOR_MEGENTA:
			om.p.color = RGB(255, 0, 255);
			break;
		case ID_COLOR_YELLOW:
			om.p.color = RGB(255, 255, 0);
			break;
		case ID_SHAPE_CIRCLE:
			om.p.isRectangle = FALSE;
			break;
		case ID_SHAPE_RECTANGLE:
			om.p.isRectangle = TRUE;
			break;
		case ID_OBJECTSIZE_20:
			om.objectMaxSize = 20;
			int tempSize = om.size; // objectMax save 
			if (om.size > om.objectMaxSize) {
				for (int i = om.objectMaxSize; i < tempSize; ++i) {
					DeleteTail(&om, i);
				}
			}
			SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
			break;
		case ID_OBJECTSIZE_25:
			om.objectMaxSize = 25;
			tempSize = om.size;
			if (om.size > om.objectMaxSize) {
				for (int i = om.objectMaxSize; i < om.size; ++i) {
					DeleteTail(&om, i);
				}
			}
			SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
			break;
		case ID_OBJECTSIZE_30:
			om.objectMaxSize = 30;
			tempSize = om.size;
			if (om.size > om.objectMaxSize) {
				for (int i = om.objectMaxSize; i < om.size; ++i) {
					DeleteTail(&om, i);
				}
			}
			SetTimer(hWnd, 1, OBJECT_TIMER, NULL);
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 4);
		KillTimer(hWnd, 5);
		KillTimer(hWnd, 6);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}



void DrawBoard(HDC hDC)
{
	for (int x = 0; x <= COUNT; ++x) {
		MoveToEx(hDC, POS(x), POS(0), NULL);
		LineTo(hDC, POS(x), POS(COUNT));
	}
	for (int y = 0; y <= COUNT; ++y) {
		MoveToEx(hDC, POS(0), POS(y), NULL);
		LineTo(hDC, POS(COUNT), POS(y));
	}
}