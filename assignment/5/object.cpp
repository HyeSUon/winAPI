#include "object.h"



Bullbasaur::Bullbasaur(int x, int y, int w, int h) : x{ x }, y{ y }, width{ w }, height{ h }, frame{ 0 } { }
Pikachu::Pikachu(int x, int y, int w, int h, int s) : x{ x }, y{ y }, width{ w }, height{ h }, frame{ 0 }, 
d{ B }, speed{ s }, isJumping{ false }, tempX{ 0 }, tempY{ 0 }, isBig{ false }, isSmall{ false }, isBallMode{ false },
isTransform{ false } {
	character = 'P';
}
MyBackGround::MyBackGround(HWND hWnd) {
	GetClientRect(hWnd, &rectView);
}
SpriteObject::SpriteObject(int x, int y, int width, int height, int frame, TCHAR c) : x{ x }, y{ y }, width{ width }, 
height{ height }, frame{ frame }, character{ c } {  

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
void Bullbasaur::Update()
{
	;
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
void MyBackGround::Update()
{
	;
}


void Pikachu::SetImage(LPCTSTR fileName, LPCTSTR fileName2, LPCTSTR fileName3)
{
	TCHAR FullName[40];
	for (int i = 0; i < PIKA_SPRITES; ++i) {
		wsprintf(FullName, L"%s%d.png", fileName, i+1);
		img[i].Load(FullName);
		imageW[i] = img[i].GetWidth();
		imageH[i] = img[i].GetHeight();
	}
	for (int i = 0; i < PIKA_SPRITES; ++i) {
		wsprintf(FullName, L"%s%d.png", fileName2, i);
		img2[i].Load(FullName);
		imageW2[i] = img2[i].GetWidth();
		imageH2[i] = img2[i].GetHeight();
	}
	for (int i = 0; i < PIKA_SPRITES; ++i) {
		wsprintf(FullName, L"%s%d.png", fileName3, i);
		img3[i].Load(FullName);
		imageW3[i] = img3[i].GetWidth();
		imageH3[i] = img3[i].GetHeight();
	}

}

void Pikachu::Update()
{
	if (isJumping) {
		static int cnt = 0;
		if (cnt < 5)
			y -= 5;
		else
			y += 5;
		cnt++;
		if (cnt == 10) {
			cnt = 0;
			isJumping = false;
		}
	}
	if (isBig) {
		static int cnt2 = 0;
		if (cnt2 < 5) {
			width += 1;
			height += 2;
		}
		else {
			width -= 1;
			height -= 2;
		}
		cnt2++;
		if (cnt2 == 10) {
			cnt2 = 0;
			isBig = false;
		}
	}
	if (isSmall) {
		static int cnt3 = 0;
		if (cnt3 < 5) {
			width -= 1;
			height -= 2;
		}
		else {
			width += 1;
			height += 2;
		}
		cnt3++;
		if (cnt3 == 10) {
			cnt3 = 0;
			isSmall = false;
		}
	}

	if (isBallMode) {
		if (d == LB) {
			x = x - cos(45) * speed;
			y = y + sin(45) * speed;
		}
		else if (d == LT) {
			x = x - cos(45) * speed;
			y = y - sin(45) * speed;
		}
		else if (d == RT) {
			x = x + cos(45) * speed;
			y = y - sin(45) * speed;
		}
		else if (d == RB) {
			x = x + cos(45) * speed;
			y = y + sin(45) * speed;
		}
		else if (d == L) {
			x = x - speed;
		}
		else if (d == R) {
			x = x + speed;
		}
		else if (d == T) {
			y = y - speed;
		}
		else if (d == B) {
			y = y + speed;
		}
	}
	if (isTransform) {
		static int cnt4 = 0;
		cnt4++;
		if (cnt4 == 10) {
			cnt4 = 0;
			isTransform = false;
			if (character == 'F')
				character = 'P';
			else if (character == 'E')
				character = 'F';
			else
				character = 'E';
		}
	}
}
void Pikachu::KeyEvent(WPARAM wParam)
{
	if (isBallMode) {
		if (wParam == 'a') {
			speed = 10;
			isBallMode = false;
		}
		return;
	}
	switch (wParam) {
	case 'j':
		tempX = x;
		tempY = y;
		isJumping = true;
		break;
	case 'e':
		isBig = true;
		break;
	case 's':
		isSmall = true;
		break;
	case 'a':
		isBallMode = true;
		speed = 15;
		break;
	}

}
void Pikachu::Move(const RECT& rView)
{
	int halfW = width / 2;
	int halfH = height / 2;
	if (x + halfW > rView.right) {
		x = rView.right - halfW;
		if (d == RT)
			d = LT;
		else
			d = LB;
	}
	else if (x - halfW < rView.left) {
		x = rView.left + halfW;
		if (d == LT)
			d = RT;
		else
			d = RB;
	}
	else if (y - halfH < rView.top) {
		y = rView.top + halfH;
		if (d == RT)
			d = RB;
		else
			d = LB;
	}
	else if (y + halfH > rView.bottom) {
		y = rView.bottom - halfH;
		if (d == RB)
			d = RT;
		else
			d = LT;
	}

	if (isJumping || isBallMode)
		return;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetKeyState(VK_LEFT) & 0x8000) {
		d = LB;
		x = x - cos(45) * speed;
		y = y + sin(45) * speed;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetKeyState(VK_UP) & 0x8000) {
		d = LT;
		x = x - cos(45) * speed;
		y = y - sin(45) * speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetKeyState(VK_RIGHT) & 0x8000) {
		d = RT;
		x = x + cos(45) * speed;
		y = y - sin(45) * speed;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetKeyState(VK_DOWN) & 0x8000) {
		d = RB;
		x = x + cos(45) * speed;
		y = y + sin(45) * speed;
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
void Pikachu::Draw(HDC memdc)
{
	int halfW = width / 2;
	int halfH = height / 2;

	// ±×¸²ÀÚ

	int h = height / 8;
	int w = width / 2;
	HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(memdc, MyBrush);
	if (isJumping) {
		Ellipse(memdc, tempX - w, tempY + 3 * h, tempX + w, tempY + 5 * h);
	}
	else {
		Ellipse(memdc, x - w, y + 3 * h, x + w, y + 5 * h);
	}
	DeleteObject(MyBrush);

	if ( (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
		(GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
		switch (d) {
		case B:
			frame = (frame++ % 2);
			break;
		case LB:
			frame = (++frame % 2) + 4;
			break;
		case L:
			frame = (frame++ % 2) + 6;
			break;
		case LT:
			frame = (++frame % 2) + 10;
			break;
		case T:
			frame = (frame++ % 2) + 12;
			break;
		case RT:
			frame = (++frame % 2) + 16;
			break;
		case R:
			frame = (frame++ % 2) + 18;
			break;
		case RB:
			frame = (++frame % 2) + 22;
			break;
		}
	}
	else {
		switch(d) {
		case B:
			frame = 0;
			break;
		case LB:
			frame = 3;
			break;
		case L:
			frame = 6;
			break;
		case LT:
			frame = 9;
			break;
		case T:
			frame = 12;
			break;
		case RT:
			frame = 15;
			break;
		case R:
			frame = 18;
			break;
		case RB:
			frame = 21;
			break;
		}
	}
	if(character == 'P')
		img[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW[frame], imageH[frame]);
	else if (character == 'E')
		img2[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW2[frame], imageH2[frame]);
	else
		img3[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, imageW3[frame], imageH3[frame]);
}
void Pikachu::MouseEvent(int mx, int my)
{
	if (character == 'P')
		character = 'F';
	else if (character == 'F')
		character = 'E';
	else
		character = 'P';

	int halfW = width / 2;
	int halfH = height / 2;
	if (mx >= x - halfW && mx <= x + halfW && my >= y - halfW && my <= y + halfW) {
		isTransform = true;
	}

}
int Pikachu::getX()
{
	return x;
}
int Pikachu::getY()
{
	return y;
}
int Pikachu::getWidth()
{
	return width;
}
int Pikachu::getHeight()
{
	return height;
}
int Pikachu::getFrame()
{
	return frame;
}
D Pikachu::getD()
{
	return d;
}

TCHAR Pikachu::getCharacter()
{
	return character;
}



void SpriteObject::Update()
{
	;
}

void SpriteObject::Draw(HDC memdc)
{
	int halfW = width / 2;
	int halfH = height / 2;
	if (character == 'P')
		Pikachu::img[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, Pikachu::imageW[frame], Pikachu::imageH[frame]);
	else if (character == 'E')
		Pikachu::img2[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, Pikachu::imageW2[frame], Pikachu::imageH2[frame]);
	else
		Pikachu::img3[frame].Draw(memdc, x - halfW, y - halfH, width, height, 0, 0, Pikachu::imageW3[frame], Pikachu::imageH3[frame]);
}