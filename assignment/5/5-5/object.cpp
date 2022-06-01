#include "object.h"




// 정적 멤버 변수 선언
CImage MyBackGround::img;
int MyBackGround::imageW, MyBackGround::imageH;
CImage Pokemon::img[POKEMON_SPRITES];
int Pokemon::imageW[POKEMON_SPRITES], Pokemon::imageH[POKEMON_SPRITES];
CImage MyBullet::img[BULLET_SPRITES];
int MyBullet::imageW[BULLET_SPRITES], MyBullet::imageH[BULLET_SPRITES];
RECT MyBackGround::rectView;

// object 생성자
Pokemon::Pokemon(int x, int y, int w, int h, int s) : x{ x }, y{ y }, width{ w }, height{ h }, frame{ 0 }, d{ B }, speed{ s } { }
MyBullet::MyBullet(int x, int y, int w, int h, D d, int s) : x{ x }, y{ y }, width{ w }, height{ h }, d{ d }, speed{ s }, frame{ 0 } { }
MyBackGround::MyBackGround(HWND hWnd) { }

// Set image
void Pokemon::SetImage(LPCTSTR fileName)
{
	TCHAR FullName[40];
	for (int i = 0; i < POKEMON_SPRITES; ++i) {
		wsprintf(FullName, L"%s%d.png", fileName, i + 1);
		img[i].Load(FullName);
		imageW[i] = img[i].GetWidth();
		imageH[i] = img[i].GetHeight();
	}
}
void MyBullet::SetImage(LPCTSTR fileName)
{
	TCHAR FullName[40];
	for (int i = 0; i < BULLET_SPRITES; ++i) {
		wsprintf(FullName, L"%s%d.png", fileName, i + 1);
		img[i].Load(FullName);
		imageW[i] = img[i].GetWidth();
		imageH[i] = img[i].GetHeight();
	}
}
void MyBackGround::SetImage(LPCTSTR fileName, const RECT& r)
{
	img.Load(fileName);
	imageW = img.GetWidth();
	imageH = img.GetHeight();
	rectView = r;
}


// Update
void MyBackGround::Update()
{
	;
}
void Pokemon::Update()
{
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
		(GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
		switch (d) {
		case B:		frame = (frame++ % 2);		break;
		case LB:	frame = (++frame % 2) + 4;	break;
		case L:		frame = (frame++ % 2) + 6;	break;
		case LT:	frame = (++frame % 2) + 10;	break;
		case T:		frame = (frame++ % 2) + 12;	break;
		case RT:	frame = (++frame % 2) + 16;	break;
		case R:		frame = (frame++ % 2) + 18;	break;
		case RB:	frame = (++frame % 2) + 22;	break;
		}
	}
	else {
		switch (d) {
		case B:		frame = 0;		break;
		case LB:	frame = 3;		break;
		case L:		frame = 6;		break;
		case LT:	frame = 9;		break;
		case T:		frame = 12;		break;
		case RT:	frame = 15;		break;
		case R:		frame = 18;		break;
		case RB:	frame = 21;		break;
		}
	}
}


void MyBullet::Update()
{

	int halfW = width / 2;
	int halfH = height / 2;

	switch (d) {
	case LB:
		x = static_cast<int>(x - cos(45) * speed);
		y = static_cast<int>(y + sin(45) * speed);
		break;
	case LT:
		x = static_cast<int>(x - cos(45) * speed);
		y = static_cast<int>(y - sin(45) * speed);
		break;
	case RT:
		x = static_cast<int>(x + cos(45) * speed);
		y = static_cast<int>(y - sin(45) * speed);
		break;
	case RB:
		x = static_cast<int>(x + cos(45) * speed);
		y = static_cast<int>(y + sin(45) * speed);
		break;
	case L:
		x = x - speed;
		break;
	case R:
		x = x + speed;
		break;
	case T:
		y = y - speed;
		break;
	case B:
		y = y + speed;
		break;
	}
	frame = ++frame % BULLET_SPRITES;
}



// Draw
void MyBackGround::Draw(HDC memdc)
{
	img.Draw(memdc, 0, 0, rectView.right, rectView.bottom, 0, 0, imageW, imageH);
}
void Pokemon::Draw(HDC memdc)
{
	int halfW = width / 2;
	int halfH = height / 2;

	// 그림자
	int h = height / 8;
	int w = width / 2;
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(memdc, MyBrush);
	Ellipse(memdc, x - w, y + 3 * h, x + w, y + 5 * h);
	DeleteObject(MyBrush);

	img[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW[frame], imageH[frame]);
}
void MyBullet::Draw(HDC memdc)
{
	int halfW = width / 2;
	int halfH = height / 2;
	img[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW[frame], imageH[frame]);
}

// Player Move
void Pokemon::Move()
{
	int halfW = width / 2;
	int halfH = height / 2;
	RECT rectView = MyBackGround::GetRectView();
	if (x + halfW > rectView.right) {
		x = rectView.right - halfW;
		if (d == RT)
			d = LT;
		else
			d = LB;
	}
	else if (x - halfW < rectView.left) {
		x = rectView.left + halfW;
		if (d == LT)
			d = RT;
		else
			d = RB;
	}
	else if (y - halfH < rectView.top) {
		y = rectView.top + halfH;
		if (d == RT)
			d = RB;
		else
			d = LB;
	}
	else if (y + halfH > rectView.bottom) {
		y = rectView.bottom - halfH;
		if (d == RB)
			d = RT;
		else
			d = LT;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetKeyState(VK_LEFT) & 0x8000) {
		d = LB;
		x = static_cast<int>(x - cos(45) * speed);
		y = static_cast<int>(y + sin(45) * speed);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetKeyState(VK_UP) & 0x8000) {
		d = LT;
		x = static_cast<int>(x - cos(45) * speed);
		y = static_cast<int>(y - sin(45) * speed);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetKeyState(VK_RIGHT) & 0x8000) {
		d = RT;
		x = static_cast<int>(x + cos(45) * speed);
		y = static_cast<int>(y - sin(45) * speed);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetKeyState(VK_DOWN) & 0x8000) {
		d = RB;
		x = static_cast<int>(x + cos(45) * speed);
		y = static_cast<int>(y + sin(45) * speed);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		d = L;
		x = x - speed;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		d = R;
		x = x + speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000) {
		d = T;
		y = y - speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		d = B;
		y = y + speed;
	}
}


bool MyBullet::isRemove()
{
	RECT rectView = MyBackGround::GetRectView();
	int halfW = width / 2;
	int halfH = height / 2;
	if (x + halfW > rectView.right || x - halfW < rectView.left || y - halfH < rectView.top || y + halfH > rectView.bottom) {
		// 자신의 위치를 알고 그 위치의 자신을 없애야함.
		return true;
	}
	return false;
}


// getter

D Pokemon::getD() {
	return d;
}
int Pokemon::getX() {
	return x;
}
int Pokemon::getY() {
	return y;
}