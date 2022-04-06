#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

#define SIZE 10

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
	circle, triangle, rectangle,
}Shape;

typedef struct {
	Shape shape; int col; int row; int size; COLORREF c; int pen;
}Data;
typedef struct {
	Data shape[SIZE]; int size; int length; int len; int cntShape; int select; BOOL isSelect; BOOL cSwitch;
	COLORREF rColor; COLORREF eColor; COLORREF tColor;
}DataManager;


void Draw(HDC hDC, const DataManager* dm);
void Update(WPARAM wParam, DataManager* dm);
void drawShape(HDC hDC, const Data* s, int len);
void CdrawShape(HDC hDC, const DataManager* dm);
void SelectDraw(HDC hDC, Data* s, int len);
void SelectUpdate(WPARAM wParam, Data* d, int size, int len);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static DataManager dm;
	static RECT r;
	srand(time(NULL));
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		dm.size = 40;
		dm.isSelect = FALSE;
		dm.cSwitch = FALSE;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		dm.len = r.bottom / dm.size;
		dm.length = dm.len * dm.size;
		Draw(hDC, &dm);

		if (dm.isSelect && dm.cntShape > 0)
			SelectDraw(hDC, &dm.shape[dm.select], dm.len);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if( dm.isSelect && dm.cntShape > 0)
			SelectUpdate(wParam, &dm.shape[dm.select], dm.size, dm.len);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		Update(wParam, &dm);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

void Update(WPARAM wParam, DataManager* dm)
{
	if (wParam == 's')
		dm->size = 30;
	else if (wParam == 'm')
		dm->size = 40;
	else if (wParam == 'l')
		dm->size = 50;


	if (wParam == 'c') {
		dm->eColor = RGB(rand() % 256, rand() % 256, rand() % 256);
		dm->rColor = RGB(rand() % 256, rand() % 256, rand() % 256);
		dm->tColor = RGB(rand() % 256, rand() % 256, rand() % 256);

		dm->cSwitch = !dm->cSwitch;
	}
	else if (wParam == '+') {
		if (dm->shape[dm->select].size > 0)
			--dm->shape[dm->select].size;
	}
	else if (wParam == '-') {
		if (dm->shape[dm->select].size < 3)
			++dm->shape[dm->select].size;
	}
	else if (wParam == 'p') {
		dm->cntShape = 0;
	}
	else if (wParam == 'q') {
		PostQuitMessage(0);
	}
	else if (dm->isSelect && wParam == 'd') {
		for (int i = dm->select; i < dm->cntShape - 1; ++i) {
			dm->shape[i] = dm->shape[i + 1];
		}
		dm->cntShape--;
		if (dm->cntShape < 0)
			dm->cntShape = 0;
	}

	if (dm->cntShape == SIZE && (wParam == 'e' || wParam == 'r' || wParam == 't')) {
		for (int i = 0; i < dm->cntShape-1; ++i) {
			dm->shape[i] = dm->shape[i + 1];
		}
		dm->cntShape--;
	}

	if (dm->cntShape < SIZE) {
		if (wParam == 'e') {
			dm->shape[dm->cntShape].shape = circle;
			dm->shape[dm->cntShape].col = rand() % dm->size;
			dm->shape[dm->cntShape].row = rand() % dm->size;
			dm->shape[dm->cntShape].c = RGB(rand() % 256, rand() % 256, rand() % 256);
			dm->shape[dm->cntShape++].size = 1;
		}
		else if (wParam == 't') {
			dm->shape[dm->cntShape].shape = triangle;
			dm->shape[dm->cntShape].col = rand() % dm->size;
			dm->shape[dm->cntShape].row = rand() % dm->size;
			dm->shape[dm->cntShape].c = RGB(rand() % 256, rand() % 256, rand() % 256);
			dm->shape[dm->cntShape++].size = 1;
		}
		else if (wParam == 'r') {
			dm->shape[dm->cntShape].shape = rectangle;
			dm->shape[dm->cntShape].col = rand() % dm->size;
			dm->shape[dm->cntShape].row = rand() % dm->size;
			dm->shape[dm->cntShape].c = RGB(rand() % 256, rand() % 256, rand() % 256);
			dm->shape[dm->cntShape++].size = 1;
		}
	}
	if (isdigit(wParam)) {
		int i = wParam - '0';
		if (i < dm->cntShape) {
			dm->select = i;
			dm->isSelect = TRUE;
		}
	}
}

void SelectUpdate(WPARAM wParam, Data* d, int size, int len)
{
	if (wParam == VK_UP) {
		if (--d->row < 0)
			d->row += size;
	}
	else if (wParam == VK_DOWN) {
		d->row = ++d->row % size;
	}
	else if (wParam == VK_LEFT) {
		if (--d->col < 0)
			d->col += size;
	}
	else if (wParam == VK_RIGHT) {
		d->col = ++d->col % size;
	}
}

void Draw(HDC hDC, const DataManager* dm)
{
	for (int i = 0; i <= dm->size; ++i) {
		MoveToEx(hDC, i * dm->len, 0, NULL);
		LineTo(hDC, i * dm->len, dm->length);
		MoveToEx(hDC, 0, i * dm->len, NULL);
		LineTo(hDC, dm->length, i * dm->len);
	}
	if (dm->cSwitch) {
		CdrawShape(hDC, dm);
	}
	else {
		for (int i = 0; i < dm->cntShape; ++i) {
			drawShape(hDC, &dm->shape[i], dm->len);
		}
	}

}

void CdrawShape(HDC hDC, const DataManager* dm)
{
	HBRUSH eBrush = CreateSolidBrush(dm->eColor);
	HBRUSH rBrush = CreateSolidBrush(dm->rColor);
	HBRUSH tBrush = CreateSolidBrush(dm->tColor);
	int len = dm->len;
	for (int i = 0; i < dm->cntShape; ++i) {
		SelectObject(hDC, eBrush);
		Data* s = &dm->shape[i];
		if (s->shape == circle) {
			Ellipse(hDC, s->col * len - s->size, s->row * len - s->size, (s->col + 1) * len + s->size, (s->row + 1) * len + s->size);
		}
		else if (s->shape == triangle) {
			SelectObject(hDC, tBrush);
			POINT point[3] = { {s->col * len + s->size, (s->row + 1) * len - s->size}, {(s->col + 1) * len - s->size, (s->row + 1) * len - s->size}, {(s->col * 2 + 1) * len / 2, s->row * len + s->size} };
			Polygon(hDC, point, 3);
		}
		else if (s->shape == rectangle) {
			SelectObject(hDC, rBrush);
			Rectangle(hDC, s->col * len + s->size, s->row * len + s->size, (s->col + 1) * len - s->size, (s->row + 1) * len - s->size);
		}
	}
	DeleteObject(eBrush);
	DeleteObject(rBrush);
	DeleteObject(tBrush);
}

void drawShape(HDC hDC, const Data* s, int len)
{
	HBRUSH hBrush = CreateSolidBrush(s->c);
	SelectObject(hDC, hBrush);
	if (s->shape == circle) {
		Ellipse(hDC, s->col * len - s->size, s->row * len - s->size, (s->col + 1) * len + s->size, (s->row + 1) * len + s->size);
	}
	else if (s->shape == triangle) {
		POINT point[3] = { {s->col * len +s->size, (s->row + 1) * len - s->size}, {(s->col + 1) * len - s->size, (s->row + 1) * len - s->size}, {(s->col * 2 + 1) * len / 2, s->row * len + s->size} };
		Polygon(hDC, point, 3);
	}
	else if (s->shape == rectangle) {
		Rectangle(hDC, s->col * len + s->size, s->row * len + s->size, (s->col + 1) * len - s->size, (s->row + 1) * len - s->size);
	}

	DeleteObject(hBrush);
}


void SelectDraw(HDC hDC, Data* s, int len)
{
	HPEN hPen = CreatePen(PS_SOLID,  3, RGB(0, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
	drawShape(hDC, s, len);
	SelectObject(hDC, oldPen);
	DeleteObject(hPen);
}