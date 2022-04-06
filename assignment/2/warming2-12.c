#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <math.h>

#define ANSWER L"apple"
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
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
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 200, 150, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

TCHAR alpha[30] = L"abcdefghigklmnopqrstuvwxyz";

typedef struct {
	RECT r;  int lenX; int lenY; TCHAR str[2]; SIZE size; COLORREF c; LPTSTR answer; unsigned char flag; int cnt;
}GameManager;

void draw(HDC hDC, GameManager* gm);
void update(WPARAM, GameManager* gm);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static GameManager gm;
	static HFONT font;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		gm.c = RGB(255, 255, 255);
		gm.answer = ANSWER;
		gm.flag = 0b0000'0000;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "궁서");
		SelectObject(hDC, font);
		GetClientRect(hWnd, &gm.r);

		gm.lenX = gm.r.right / 10;  // 비율을 맞추는 변수
		gm.lenY = gm.r.bottom / 4; // 비율을 맞추는 변수

		draw(hDC, &gm);

		DeleteObject(font);
		if (gm.cnt == 5) {
			if(IDOK == MessageBox(hWnd, L"GAME OVER", L"You Lose", MB_ICONWARNING | MB_OK))
				PostQuitMessage(0);
		}
		if (gm.flag == 0b0001'1111) {
			if (IDOK == MessageBox(hWnd, L"CLEAR", L"You Win!", MB_ICONWARNING | MB_OK))
				PostQuitMessage(0);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		update(wParam, &gm);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}



void draw(HDC hDC, GameManager* gm)
{
	// hangman
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 100, 255));
	SelectObject(hDC, hBrush);

	Rectangle(hDC, gm->lenX * 6, gm->lenY * 3, gm->lenX * 10, gm->lenY * 4);
	MoveToEx(hDC, gm->lenX * 6, gm->lenY * 3, NULL);
	LineTo(hDC,gm->lenX * 6, 0);
	MoveToEx(hDC, gm->lenX * 6, 0, NULL);
	LineTo(hDC, gm->lenX * 8, 0);
	MoveToEx(hDC, gm->lenX * 8, 0, NULL);
	LineTo(hDC, gm->lenX * 8, gm->lenY/2);
	if(gm->cnt >= 1)
		Ellipse(hDC, gm->lenX * 7.5, gm->lenY / 2, gm->lenX * 8.5, gm->lenY);
	if (gm->cnt >= 2) {
		MoveToEx(hDC, gm->lenX * 8, gm->lenY, NULL);
		LineTo(hDC, gm->lenX * 8, gm->lenY * 2);
	}
	if (gm->cnt >= 3) {
		MoveToEx(hDC, gm->lenX * 8, gm->lenY*1.3, NULL);
		LineTo(hDC, gm->lenX * 7, gm->lenY *1.8);
	}
	if (gm->cnt >= 4) {
		MoveToEx(hDC, gm->lenX * 8, gm->lenY * 1.3, NULL);
		LineTo(hDC, gm->lenX * 9, gm->lenY * 1.8);
	}
	if (gm->cnt >= 5) {
		MoveToEx(hDC, gm->lenX * 8, gm->lenY * 2, NULL);
		LineTo(hDC, gm->lenX * 9, gm->lenY * 2.5);
		MoveToEx(hDC, gm->lenX * 8, gm->lenY * 2, NULL);
		LineTo(hDC, gm->lenX * 7, gm->lenY * 2.5);
	}

	DeleteObject(hBrush);
	GetTextExtentPoint32(hDC, gm->str, 1, &gm->size);
	TextOut(hDC, gm->lenX * 2, gm->lenY * 3, gm->str, wcslen(gm->str));
	MoveToEx(hDC, gm->lenX * 2, gm->lenY*3+gm->size.cy, NULL);
	LineTo(hDC, gm->lenX * 2 + gm->size.cx, gm->lenY * 3 + gm->size.cy);


	gm->c = RGB(255 - 50 * gm->cnt, 255 - 50 * gm->cnt, 255 - 50 * gm->cnt);
	HBRUSH sBrush = CreateSolidBrush(gm->c);

	SelectObject(hDC, sBrush);
	SetBkMode(hDC, TRANSPARENT);
	for (int i = 0; i < 5; ++i) {
		Rectangle(hDC, gm->lenX * i, gm->lenY, gm->lenX * (i + 1), gm->lenY*1.5);
	}


	unsigned char tempFlag = gm->flag;
	for (int i = 0; i < 5; ++i) {
		if (tempFlag & (int)pow(2, i))
			TextOut(hDC, gm->lenX * i + 20, gm->lenY + 10, &gm->answer[i], 1);
	}
	DeleteObject(sBrush);
}


void update(WPARAM wParam, GameManager* gm)
{
	int i;
	for (i = 0; i < 27; ++i) {
		if (wParam == alpha[i])
			break;
	}
	if (i == 27)
		return;
	if (isalpha(wParam)) {
		gm->str[0] = wParam;
		gm->str[1] = '\0';
	}
	BOOL AnswerCheck = FALSE;
	for (int i = 0; i < 5; ++i) {
		if (wParam == gm->answer[i]) {
			gm->flag |= (int)pow(2, i);
			alpha[gm->answer[i] - 'a'] = '0';
			AnswerCheck = TRUE;
		}
	}
	if (!AnswerCheck) {
		if( gm->cnt < 5)
			gm->cnt++;
	}

	
}
