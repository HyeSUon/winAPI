#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>


typedef struct {
	int xlen; int ylen; SIZE size; TCHAR ch; int num;
}Data;



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

void setData(HDC hDC, RECT r, Data* d);
void shapeX(HDC hDC, RECT r);
void invertedTriangle(HDC hDC, RECT r);
void rhombus(HDC hDC, RECT r);
void butterfly(HDC hDC, RECT r);
void hourglass(HDC hDC, RECT r);
void square(HDC hDC, RECT r);


typedef void (*funcPtr[6])(HDC hDC, RECT r);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static RECT r;
	static int rnum, cnum;
	static int shape;
	funcPtr fp = { shapeX, invertedTriangle, rhombus, butterfly, hourglass, square };
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		srand(time(NULL));
		rnum = rand() % 9 + 2;
		cnum = rand() % 9 + 2;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		rnum = rand() % 9 + 2;
		cnum = rand() % 9 + 2;
		rnum = r.bottom / rnum;
		cnum = r.right / cnum;
		for (int row = 0; row < r.bottom; row+= rnum) {
			for (int col = 0; col < r.right; col += cnum) {
				RECT size = { col, row, col + cnum, row + rnum };
				shape = rand() % 6;
				fp[shape](hDC, size);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		rnum = rand() % 9 + 2;
		cnum = rand() % 9 + 2;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

void setData(HDC hDC, RECT r, Data* d)
{
	d->ch = rand() % 10 + '0';
	GetTextExtentPoint32(hDC, &(d->ch), 1, &(d->size));
	d->xlen = d->size.cx;
	d->ylen = d->size.cy;
	if ((r.right - r.left) >= (r.bottom - r.top)) {
		d->num = (r.right - r.left) / d->xlen;
		d->ylen = (r.bottom - r.top) / d->num;
	}
	else {
		d->num = (r.bottom - r.top) / d->ylen;
		d->xlen = (r.right - r.left) / d->num;
	}
	SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
	SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
}

void shapeX(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);
	for (int i = 0; i < d.num; ++i) {
		for (int j = 0; j < d.num; ++j) {
			if (i == j || i+j == d.num - 1) {
				TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
			}
		}
	}
}

void invertedTriangle(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);

	for (int i = 0; i < d.num; ++i) {
		for (int j = 0; j < d.num; ++j) {
			if (j < i)
				;
			else if (j < d.num - i) {
				TextOut(hDC, r.left + j * d.xlen, r.top + i * 2 * d.ylen, &d.ch, 1);
				TextOut(hDC, r.left + j * d.xlen, r.top + (i * 2+1) * d.ylen, &d.ch, 1);
			}
		}
	}
}

void rhombus(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);

	int cnt = d.num / 2;
	for (int i = 0; i < d.num; ++i) {
		for (int j = 0; j < d.num; ++j) {
			if (i < d.num / 2) {
				if (j >= d.num / 2 - i && j <= d.num / 2 + i) {
					TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
				}
			}
			else {
				if ( j >= d.num / 2 - cnt && j <= d.num / 2 + cnt)
					TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
			}
					
			
		}

		if(i >= d.num / 2)
			cnt--;
	}

}

void butterfly(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);
	int cnt = d.num / 2;
	for (int i = 1; i < d.num; ++i) {
		for (int j = 0; j < d.num; ++j) {
			if (i < d.num / 2) {
				if (j >= d.num / 2 - cnt && j <= d.num / 2 + cnt)
					TextOut(hDC, r.left + i * d.xlen, r.top + j * d.ylen, &d.ch, 1);
			}
			else {
				if (j >= d.num / 2 - cnt && j <= d.num / 2 + cnt)
					TextOut(hDC, r.left + i * d.xlen, r.top + j * d.ylen, &d.ch, 1);
			}


		}

		if (i < d.num / 2)
			cnt--;
		else
			cnt++;
	}
}
void hourglass(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);
	int cnt = d.num / 2;
	for (int i = 0; i < d.num; ++i) {
		for (int j = 1; j < d.num; ++j) {
			if (i < d.num / 2) {
				if (j >= d.num / 2 - cnt && j <= d.num / 2 + cnt)
					TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
			}
			else {
				if (j >= d.num / 2 - cnt && j <= d.num / 2 + cnt)
					TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
			}


		}

		if (i < d.num / 2)
			cnt--;
		else
			cnt++;
	}
}

void square(HDC hDC, RECT r)
{
	Data d;
	setData(hDC, r, &d);




	int h = d.num / 2;

	for (int i = 0; i < d.num; ++i) {
		for (int j = 0; j < d.num; ++j) {
			if( i == 0 || i == d.num-1)
				TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
			else if (j == 0 || j == d.num - 1)
				TextOut(hDC, r.left + j * d.xlen, r.top + i * d.ylen, &d.ch, 1);
		}
	}
}