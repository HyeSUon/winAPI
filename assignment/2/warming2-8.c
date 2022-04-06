#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>





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

void hourglass(HDC hDC, RECT r, int col, int row, BOOL);
void pentagon(HDC hDC, RECT r, int col, int row, BOOL big);
void triangle(HDC hDC, RECT r, int col, int row, BOOL big);
void pie(HDC hDC, RECT r, int col, int row, BOOL big);
void CenterRect(HDC hDC, RECT r);

typedef void (*funcPtr[4])(HDC hDC, RECT r, int col, int row, BOOL big);
typedef enum {
	Hourglass, Pentagon, Triangle, PIE
}Shape;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static BOOL big;
	static RECT r;
	HBRUSH hBrush[4];
	static COLORREF c[4];
	static COLORREF oldc;
	static BOOL ColorChanged;
	static Shape shape;
	funcPtr fp = { hourglass, pentagon, triangle, pie };
	POINT pt[4] = { {1,0}, {2,1}, {0, 1}, {1,2} };
	
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		srand(time(NULL));
		ColorChanged = FALSE;
		for (int i = 0; i < 4; ++i) {
			c[i] = RGB(rand() % 256, rand() % 256,  rand() % 256);
		}
		big = TRUE;
		shape = rand() % 4;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		CenterRect(hDC, r);
		for (int i = 0; i < 4; ++i) {
			hBrush[i] = CreateSolidBrush(c[i]);
			SelectObject(hDC, hBrush[i]);
			fp[i](hDC, r, pt[i].x, pt[i].y, big);
		}
		hBrush[shape] = CreateSolidBrush(c[shape]);
		SelectObject(hDC, hBrush[shape]);
		fp[shape](hDC, r, 1, 1, big); //center

		for (int i = 0; i < 4; ++i)
			DeleteObject(hBrush[i]);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == 't' || wParam == 'T') {
			big = TRUE;
			shape = Triangle;
			if (!ColorChanged) {
				oldc = c[shape];
				c[shape] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			ColorChanged = TRUE;
		}
		else if (wParam == 's' || wParam == 'S') {
			big = TRUE;
			shape = Hourglass;
			if (!ColorChanged) {
				oldc = c[shape];
				c[shape] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			ColorChanged = TRUE;
		}
		else if (wParam == 'p' || wParam == 'P') {
			big = TRUE;
			shape = Pentagon;
			if (!ColorChanged) {
				oldc = c[shape];
				c[shape] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			ColorChanged = TRUE;
		}
		else if (wParam == 'e' || wParam == 'E') {
			big = TRUE;
			shape = PIE;
			if (!ColorChanged) {
				oldc = c[shape];
				c[shape] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			ColorChanged = TRUE;
		}
		else if (wParam == VK_UP) {
			shape = Hourglass;
			big = FALSE;
		}
		else if (wParam == VK_DOWN) {
			shape = PIE;
			big = FALSE;
		}
		else if (wParam == VK_RIGHT) {
			shape = Pentagon;
			big = FALSE;
		}
		else if (wParam == VK_LEFT) {
			shape = Triangle;
			big = FALSE;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYUP:
		if (wParam == 't' || wParam == 'T') {
			c[shape] = oldc;
			ColorChanged = FALSE;
		}
		else if (wParam == 's' || wParam == 'S') {
			c[shape] = oldc;
			ColorChanged = FALSE;
		}
		else if (wParam == 'p' || wParam == 'P') {
			c[shape] = oldc;
			ColorChanged = FALSE;
		}
		else if (wParam == 'e' || wParam == 'E') {
			c[shape] = oldc;
			ColorChanged = FALSE;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

void hourglass(HDC hDC, RECT r, int col, int row, BOOL big)
{
	int cnum = r.right / 3;
	int rnum = r.bottom / 3;

	int size = 30;
	if (col == 1 && row == 1 && !big)
		size = 70;
	RECT rect = { col * cnum + size+20, row * rnum +size, (col + 1) * cnum - size-20, (row + 1) * rnum-size };
	POINT point[4] = { {rect.left, rect.top}, {rect.right, rect.bottom}, {rect.left, rect.bottom}, {rect.right, rect.top} };
	Polygon(hDC, point, 4);
}

void pentagon(HDC hDC, RECT r, int col, int row, BOOL big)
{
	int cnum = r.right / 3 ;
	int rnum = r.bottom / 3;

	int size = 30;
	if (col == 1 && row == 1 && !big)
		size = 70;
	RECT rect = { col * cnum + size+20, row * rnum + size, (col + 1) * cnum - size-20, (row + 1) * rnum - size };
	SIZE wth = { rect.right - rect.left, rect.bottom - rect.top };
	POINT point[5] = { {rect.left + wth.cx / 2, rect.top }, {rect.right, rect.top + wth.cy * 0.4 }, 
		{ rect.left+ wth.cx * 0.8, rect.bottom}, {rect.left + wth.cx * 0.2, rect.bottom}, 
		{ rect.left, rect.top + wth.cy * 0.4} };
	Polygon(hDC, point, 5);
}

void triangle(HDC hDC, RECT r, int col, int row, BOOL big)
{
	int cnum = r.right / 3;
	int rnum = r.bottom / 3;

	int size = 30;
	if (col == 1 && row == 1 && !big)
		size = 70;
	RECT rect = { col * cnum + size+20, row * rnum + size, (col + 1) * cnum - size-20, (row + 1) * rnum - size };
	POINT point[3] = { {(rect.right - rect.left) / 2 + rect.left,rect.top }, {rect.left, rect.bottom},
		{rect.right, rect.bottom} };
	Polygon(hDC, point, 3);
}

void pie(HDC hDC, RECT r, int col, int row, BOOL big)
{
	int cnum = r.right / 3;
	int rnum = r.bottom / 3;

	int size = 30;
	if (col == 1 && row == 1 && !big)
		size = 70;
	RECT rect = { col * cnum + size + 20, row * rnum + size, (col + 1) * cnum - size - 20, (row + 1) * rnum - size };
	POINT point[3] = { {(rect.right - rect.left) / 2 + rect.left,rect.top }, {rect.left, rect.bottom},
		{rect.right, rect.bottom} };
	Pie(hDC, rect.left, rect.top, rect.right, rect.bottom, (rect.right - rect.left) / 2 + rect.left, rect.top,
		rect.right, (rect.bottom - rect.top) / 2 + rect.top);
}

void CenterRect(HDC hDC, RECT r)
{
	int cnum = r.right / 3;
	int rnum = r.bottom / 3;
	RECT rect = { cnum, rnum, cnum *2, rnum*2 };
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hDC, hBrush);
	FrameRect(hDC, &rect, hBrush);
	DeleteObject(hBrush);
}