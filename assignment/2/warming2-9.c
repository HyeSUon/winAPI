#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#define randomize() srand((unsigned)time(NULL))
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


typedef enum {
	white, red, green, yellow, orange,
}State;
typedef enum {
	circle, triangle, rectangle,
}Shape;
typedef struct {
	int col; int row; COLORREF c; Shape s; BOOL big;
}Player;
typedef struct {
	State fd[40][40]; int length; int len; Player p1; Player p2; BOOL turn; int error;
}DataManager;

void drawField(HDC hDC, const DataManager* dm);
void init(DataManager* dm, RECT* r);
void drawPlayer(HDC hDC, const Player* p, int len);
void PlayerControl(WPARAM wParam, DataManager* dm);
void itemUpdate(DataManager* dm, Player* p);
void printError(HDC hDC, int error, int length);
State ThisFieldState(State fd[40][40], Player* p);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static DataManager dm;
	static RECT r;
	//--- 메시지 처리하기

	randomize();
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &r);
		init(&dm, &r);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		drawField(hDC, &dm);
		printError(hDC, dm.error, dm.length);
		dm.error = 0;

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == 'r')
			init(&dm, &r);
		if(wParam == 'q')
			PostQuitMessage(0);
		PlayerControl(wParam, &dm);
		itemUpdate(&dm, &dm.p1);
		itemUpdate(&dm, &dm.p2);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

void printError(HDC hDC, int error, int length)
{
	TextOut(hDC, length *1.05, 0, L"* 오류 메세지 출력 * ", wcslen(L"* 오류 메세지 출력 * "));
	if (error == -1) {
		TextOut(hDC, length, 20, L"p2 차례입니다. (ijkl)", wcslen(L"p2 차례입니다. (ijkl)"));
	}
	else if (error == -2) {
		TextOut(hDC, length, 20, L"p1 차례입니다. (ijkl)", wcslen(L"p1 차례입니다. (wasd)"));
	}
}

void itemUpdate(DataManager* dm, Player* p)
{
	if (ThisFieldState(dm->fd, p) == white)
		;
	else if (ThisFieldState(dm->fd, p) == green) {
		p->c = RGB(rand() % 256, rand() % 256, rand() % 256);
		dm->fd[p->row][p->col] = white;
	}
	else if (ThisFieldState(dm->fd, p) == yellow) {
		p->big = !p->big;
		dm->fd[p->row][p->col] = white;
	} 
	else if (ThisFieldState(dm->fd, p) == orange) {
		p->s = (p->s + 1) % 3;
		dm->fd[p->row][p->col] = white;
	}
}

State ThisFieldState(State fd[40][40], Player* p)
{
	return fd[p->row][p->col];
}
void PlayerControl(WPARAM wParam, DataManager* dm)
{
	if (dm->turn) {
		if (wParam == 'w') {
			if (dm->p1.row == 0)
				dm->p1.row = 39;
			else
				--dm->p1.row;
			dm->turn = FALSE;
			

			if (dm->fd[dm->p1.row][dm->p1.col] == red) {
				dm->p1.row = (dm->p1.row + 1) % 40;
				dm->turn = TRUE;
			}

		}
		else if (wParam == 'a') {
			if (dm->p1.col == 0)
				dm->p1.col = 39;
			else
				--dm->p1.col;
			dm->turn = FALSE;

			if (dm->fd[dm->p1.row][dm->p1.col] == red) {
				dm->p1.col = (dm->p1.col + 1) % 40;
				dm->turn = TRUE;
			}
		}
		else if (wParam == 's') {
			dm->p1.row = (dm->p1.row + 1) % 40;
			dm->turn = FALSE;

			if (dm->fd[dm->p1.row][dm->p1.col] == red) {
				if (dm->p1.row == 0)
					dm->p1.row = 39;
				else
					--dm->p1.row;
				dm->turn = TRUE;
			}
		}
		else if (wParam == 'd') {
			dm->p1.col = (dm->p1.col + 1) % 40;
			dm->turn = FALSE;
			if (dm->fd[dm->p1.row][dm->p1.col] == red) {
				if (dm->p1.col == 0)
					dm->p1.col = 39;
				else
					--dm->p1.col;
				dm->turn = TRUE;
			}
		}
		else if (wParam == 'j' || wParam == 'k' || wParam == 'l' || wParam == 'i')
			dm->error = -1;
	}
	else {
		if (wParam == 'i') {
			if (dm->p2.row == 0)
				dm->p2.row = 39;
			else
				--dm->p2.row;
			dm->turn = TRUE;
			if (dm->fd[dm->p2.row][dm->p2.col] == red) {
				dm->p2.row = (dm->p2.row + 1) % 40;
				dm->turn = FALSE;
			}

		}
		else if (wParam == 'j') {
			if (dm->p2.col == 0)
				dm->p2.col = 39;
			else
				--dm->p2.col;
			dm->turn = TRUE;

			if (dm->fd[dm->p2.row][dm->p2.col] == red) {
				dm->p2.col = (dm->p2.col + 1) % 40;
				dm->turn = FALSE;
			}
		}
		else if (wParam == 'k') {
			dm->p2.row = (dm->p2.row + 1) % 40;
			dm->turn = TRUE;

			if (dm->fd[dm->p2.row][dm->p2.col] == red) {
				if (dm->p2.row == 0)
					dm->p2.row = 39;
				else
					--dm->p2.row;
				dm->turn = FALSE;
			}
		}
		else if (wParam == 'l') {
			dm->p2.col = (dm->p2.col + 1) % 40;
			dm->turn = TRUE;

			if (dm->fd[dm->p2.row][dm->p2.col] == red) {
				if (dm->p2.col == 0)
					dm->p2.col = 39;
				else
					--dm->p2.col;
				dm->turn = FALSE;
			}
		}
		else if (wParam == 'w' || wParam == 'a' || wParam == 's' || wParam == 'd')
			dm->error = -2;
	}
}

void init(DataManager* dm, RECT* r)
{
	dm->turn = TRUE;
	dm->length = r->bottom;
	dm->len = dm->length / 40;
	int itemCnt = rand() % 100 + 20;
	for (int i = 0; i < itemCnt; ++i) {
		int r = rand() % 1600;
		if(*((*dm->fd) +r) == white)
			*((*dm->fd) + r) = (State)(rand() % 4 + 1);
		else
			i--;
	}

	dm->p1.col = rand() % 40;
	dm->p1.row = rand() % 40;
	dm->p2.col = rand() % 40;
	dm->p2.row = rand() % 40;
	while (dm->fd[dm->p1.col][dm->p1.row] != white) {
		dm->p1.col = rand() % 40;
		dm->p1.row = rand() % 40;
	}
	while (dm->fd[dm->p2.col][dm->p2.row] != white) {
		dm->p2.col = rand() % 40;
		dm->p2.row = rand() % 40;
	}

	dm->p1.c = RGB(rand() % 256, rand() % 256, rand() % 256);
	dm->p2.c = RGB(rand() % 256, rand() % 256, rand() % 256);
	dm->p1.s = rectangle;
	dm->p2.s = triangle;
	dm->p1.big = TRUE;
	dm->p2.big = TRUE;
	dm->error = 0;
}


void drawField(HDC hDC,const DataManager* dm)
{

	for (int i = 0; i < dm->length; i += dm->len) {
		MoveToEx(hDC, i, 0, NULL);
		LineTo(hDC, i, dm->length);
		MoveToEx(hDC, 0, i, NULL);
		LineTo(hDC, dm->length, i);
	}
	
	HBRUSH hRed = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hGreen = CreateSolidBrush(RGB(0, 125, 0));
	HBRUSH hYellow = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH hOrange = CreateSolidBrush(RGB(255, 125, 0));
	HBRUSH hBlack = (HBRUSH) GetStockObject(BLACK_BRUSH);

	int len = dm->len;
	for (int row = 0; row < 40; ++row) {
		for (int col = 0; col < 40; ++col) {
			if (dm->fd[row][col] == white)
				;
			else if (dm->fd[row][col] == red) {
				FillRect(hDC, &(RECT){col* len, row* len, (col + 1)* len, (row + 1)* len}, hRed);		
			} else if (dm->fd[row][col] == green) {
				FillRect(hDC, &(RECT){col* len, row* len, (col + 1)* len, (row + 1)* len}, hGreen);
			}
			else if (dm->fd[row][col] == yellow) {
				FillRect(hDC, &(RECT){col* len, row* len, (col + 1)* len, (row + 1)* len}, hYellow);
			}
			else if (dm->fd[row][col] == orange) {
				FillRect(hDC, &(RECT){col* len, row* len, (col + 1)* len, (row + 1)* len}, hOrange);
			}
		}
	}
	DeleteObject(hRed);
	DeleteObject(hGreen);
	DeleteObject(hYellow);
	DeleteObject(hOrange);


	drawPlayer(hDC, &(dm->p1), len);
	drawPlayer(hDC, &(dm->p2), len);
}

void drawPlayer(HDC hDC, const Player* p, int len)
{
	HBRUSH pBrush = CreateSolidBrush(p->c);
	int col = p->col;
	int row = p->row;
	SelectObject(hDC, pBrush);
	int a; // size control

	if (p->big)
		a = 1;
	else
		a = 3;
	if (p->s == circle) {
		Ellipse(hDC, col * len + a, row * len + a, (col + 1) * len - a, (row + 1) * len - a);
	}
	else if (p->s == triangle) {
		POINT pt[3] = { {col * len + a, (row + 1) * len - a}, {(col + 1) * len - a, (row + 1) * len - a}, { (col * len + (col + 1) * len) / 2, row * len + a} };
		Polygon(hDC, pt, 3);
	}
	else if (p->s == rectangle) {
		Rectangle(hDC, col * len + a, row * len + a, (col + 1) * len - a, (row + 1) * len - a);
	}
	DeleteObject(pBrush);
}