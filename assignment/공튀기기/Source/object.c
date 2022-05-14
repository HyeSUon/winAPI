#include "object.h"




void InitObject(ObjectManager* om)
{
	srand(time(NULL));
	om->blockMaxLine = BLOCK_MAX_LINE;
	om->blockMaxSize = om->blockMaxLine * BLOCK_MAX_COUNT;
	
	om->stick.size = 50;
	om->stick.speed = 10;
	om->stick.r.left = om->stick.oldR.left = om->rClient.right / 2 - om->stick.size;
	om->stick.r.right = om->stick.oldR.right = om->rClient.right / 2 + om->stick.size;
	om->stick.r.top = om->stick.oldR.top = om->rClient.bottom - 15;
	om->stick.r.bottom = om->stick.oldR.bottom = om->rClient.bottom;
	om->ball.c = RGB(0, 0, 255);
	om->ball.theta = 0.8;
	om->ball.r = 15;
	om->ball.a = 10;
	om->ball.x = om->stick.r.right - om->stick.size;
	om->ball.y = om->stick.r.top - om->ball.r;


	// blocks
	double px = (om->rClient.right - om->rClient.left) / (BLOCK_MAX_COUNT + 2);
	double py = (om->rClient.bottom - om->rClient.top) / 3 / BLOCK_MAX_LINE;

	for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
		for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
			om->block[row][col].r.left = px * (col + 1);
			om->block[row][col].r.right = px * (col + 2);
			om->block[row][col].r.top = py * row;
			om->block[row][col].r.bottom = py * (row + 1);
		}
	}
}




void MoveBall(Ball* ball)
{
	ball->x += cos(ball->theta * PI) * ball->a;
	ball->y -= sin(ball->theta * PI) * ball->a;
}
void MoveStick(WPARAM wParam, Stick* stick)
{
	switch (wParam)
	{
	case VK_LEFT:
		stick->r.left -= stick->speed;
		stick->r.right -= stick->speed;
		break;
	case VK_RIGHT:
		stick->r.left += stick->speed;
		stick->r.right += stick->speed;
		break;
	}
}


void MoveBlocks(Block blocks[BLOCK_MAX_LINE][BLOCK_MAX_COUNT], BOOL* isLeft, RECT cr)
{
	int speed = 1;
	if (*isLeft) {
		for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
			for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
				blocks[row][col].r.left -= speed;
				blocks[row][col].r.right -= speed;
			}
		}
	}
	else {
		for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
			for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
				blocks[row][col].r.left += speed;
				blocks[row][col].r.right += speed;
			}
		}
	}
	for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
		for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
			if (blocks[row][col].r.left < 0) {
				*isLeft = FALSE;
				break;
			}
			if (blocks[row][col].r.right > cr.right) {
				*isLeft = TRUE;
				break;
			}
		}

	}
}
void InFingerSnap(Ball* ball, const RECT* r, BOOL isLeft, BOOL isTop, BOOL isRight, BOOL isBottom)
{

	// 벽 충돌 판정
	if (ball->y > r->top && ball->y < r->bottom) {
		if (isLeft) {
			if (ball->x - ball->r < r->left) { // left
				ball->theta = 1 - ball->theta;
				if (ball->theta > 1)
					ball->theta += 2;
				ball->x = r->left + ball->r;
			}
		}
		if (isRight) {
			if (ball->x + ball->r > r->right) // right
			{
				ball->theta = 1 - ball->theta;
				if (ball->theta > 1)
					ball->theta += 2;
				ball->x = r->right - ball->r;
			}
		}
	}
	if (ball->x > r->left && ball->x < r->right) {
		if (isTop) {
			if (ball->y - ball->r < r->top) { // top
				ball->theta = 2 - ball->theta;
				ball->y = r->top + ball->r;
			}
		}
		if (isBottom) {
			if (ball->y + ball->r > r->bottom) { // bottom
				ball->theta = 2 - ball->theta;
				ball->y = r->bottom + ball->r;
			}
		}
	}
}

BOOL CollideCirRec(RECT* r, Ball* ball) {
	if (!(r->bottom + ball->r > ball->y && r->top - ball->r < ball->y &&
		r->right + ball->r > ball->x && r->left - ball->r < ball->x))
		return FALSE; // 원, 사각형 접하지 않는다면 FALSE 리턴 (대각선에서 살짝 오차가 있음)

	// 대각선 예외 처리
	if (IsPointBall(ball, r->right, r->top)) {
		ball->theta = 1 - ball->theta;
		MoveBall(ball);
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->top)) {
		ball->theta = 1 - ball->theta;
		MoveBall(ball);
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->bottom)) {
		ball->theta = 1 + ball->theta;
		MoveBall(ball);
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->bottom)) {
		ball->theta = 1 + ball->theta;
		MoveBall(ball);
		return TRUE;
	}

	// 팅기기 ( 대략적인 계산)
	int cx = (r->right + r->left) / 2;
	int cy = (r->bottom + r->top) / 2;

	int subX = cx - ball->x;
	int subY = cy - ball->y;

	if (abs(subX) > abs(subY)) {
		if (subX > 0) { 		// RIGHT
			ball->theta = 1 - ball->theta;
			if (ball->theta > 1)
				ball->theta += 2;
			MoveBall(ball);
			return TRUE;
		}
		else {					//LEFT
			ball->theta = 1 - ball->theta;
			if (ball->theta > 1)
				ball->theta *= -1;
			MoveBall(ball);
			return TRUE;
		}
	}
	else {
		if (subY > 0) {			// bottom
			ball->theta = 2 - ball->theta;
			MoveBall(ball);
			return TRUE;
		}


		else {					// TOP
			ball->theta = 2 - ball->theta;
			MoveBall(ball);
			return TRUE;
		}

	}
	return FALSE;
}
BOOL IsPointBall( Ball* b, int x, int y)
{
	float deltaX = b->x - x;
	float deltaY = b->y - y;
	float length = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	if (length > b->r)
		return FALSE;
	return TRUE;
}


BOOL OutFingerSnap(Ball* ball, const RECT* r, BOOL isLeft, BOOL isTop, BOOL isRight, BOOL isBottom)
{
	if (!(r->bottom + ball->r > ball->y && r->top - ball->r < ball->y &&
		r->right + ball->r > ball->x && r->left - ball->r < ball->x))
		return FALSE; // 원, 사각형 접하지 않는다면 FALSE 리턴 (대각선에서 살짝 오차가 있음)

	if (IsPointBall(ball, r->right, r->top)) {
		ball->theta = 1 - ball->theta;
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->top)) {
		ball->theta = 1 - ball->theta;
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->bottom)) {
		ball->theta = 1 + ball->theta;
		return TRUE;
	}
	if (IsPointBall(ball, r->left, r->bottom)) {
		ball->theta = 1 + ball->theta;
		return TRUE;
	}
	
	ball->theta = 2 - ball->theta;
	if (abs(ball->theta - 0.5) <= 0.001) {
	ball->theta = (double)(rand() % 50) / 100 + 0.2;
	ball->y = r->top - ball->r;
	}
	return TRUE;

	//if (ball->y > r->top && ball->y < r->bottom) {


	//	if (isLeft) {
	//		if (ball->x + ball->r > r->right) { // left
	//			ball->theta = 1 - ball->theta;
	//			if (ball->theta > 1)
	//				ball->theta *= -1;
	//			return TRUE;
	//		}
	//	}
	//	if (isRight) {
	//		if (ball->x - ball->r < r->left)// right
	//		{
	//			ball->theta = 1 - ball->theta;
	//			if (ball->theta > 1)
	//				ball->theta += 2;
	//			return TRUE;
	//		}
	//	}
	//}
	//if (ball->x > r->left && ball->x < r->right) {
	//	if (isTop) {
	//		if (ball->y + ball->r > r->top) { // top
	//			ball->theta = 2 - ball->theta;
	//			if (abs(ball->theta - 0.5) <= 0.001) {
	//				ball->theta = (double)(rand() % 50) / 100 + 0.2;
	//				//ball->y = r->top - ball->r;
	//			}
	//			return TRUE;
	//		}
	//	}
	//	if (isBottom) {
	//		if (ball->y - ball->r < r->bottom) { // bottom
	//			ball->theta = 2 - ball->theta;
	//			return TRUE;
	//		}
	//	}
	//}
	// return FALSE;
}

void DrawBall(HDC hDC, const Ball* ball)
{
	HBRUSH hBrush = CreateSolidBrush(ball->c);
	SelectObject(hDC, hBrush);
	Ellipse(hDC, ball->x - ball->r, ball->y - ball->r, ball->x + ball->r, ball->y + ball->r);
	DeleteObject(hBrush);
}

void DrawStick(HDC hDC, const Stick* stick)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hDC, hBrush);
	SelectObject(hDC, GetStockObject(WHITE_PEN));
	Rectangle(hDC, stick->r.left, stick->r.top, stick->r.right, stick->r.bottom);
	DeleteObject(hBrush);
}

void DrawROPStick(HDC hDC, LPARAM lParam, Stick* stick)
{
	stick->r.left = LOWORD(lParam) - stick->size;
	stick->r.right = LOWORD(lParam) + stick->size;

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hDC, hBrush);
	SetROP2(hDC, R2_XORPEN);
	Rectangle(hDC, stick->oldR.left, stick->oldR.top, stick->oldR.right, stick->oldR.bottom);
	Rectangle(hDC, stick->r.left, stick->r.top, stick->r.right, stick->r.bottom);
	DeleteObject(hBrush);
	stick->oldR.left = stick->r.left;
	stick->oldR.right = stick->r.right;
}

void DrawBlocks(HDC hDC, const Block block[BLOCK_MAX_LINE][BLOCK_MAX_COUNT])
{
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 0));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 255, 255));
	for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
		for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
			if(block[row][col].isSmall)
				SelectObject(hDC, hBrush2);
			else
				SelectObject(hDC, hBrush);
			Rectangle(hDC, block[row][col].r.left, block[row][col].r.top, block[row][col].r.right, block[row][col].r.bottom);
		}
		}
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
}



void CollideBlock(Block block[][BLOCK_MAX_COUNT], const Ball* ball, int* broken, int* smallCnt)
{
	for (int row = 0; row < BLOCK_MAX_LINE; ++row) {
		for (int col = 0; col < BLOCK_MAX_COUNT; ++col) {
			
			if (CollideCirRec(&block[row][col].r, ball)) {
				if (block[row][col].isSmall) { // temp setting
					block[row][col].r.left = 5000;
					block[row][col].r.right = 5000;
					block[row][col].r.bottom = 5000;
					block[row][col].r.top = 5000;
					++(*broken);
					--(*smallCnt);
				}
				else {
					block[row][col].isSmall = TRUE;
					int size = 10;
					block[row][col].r.left += size;
					block[row][col].r.right -= size;
					block[row][col].r.bottom -= size;
					block[row][col].r.top += size;
					++(*smallCnt);
				}
			};

		}
	}
}
BOOL FallBall(Ball* ball, const Stick* stick, RECT cr)
{
	if (ball->y - ball->r > cr.bottom) {
		ball->x = stick->r.right - stick->size;
		ball->y = stick->r.top - ball->r - 30;
		ball->theta = 1.5;
		return FALSE;
	}
	return TRUE;
}

BOOL InRectangle(RECT r, int mx, int my)
{
	if (mx >= r.left && mx <= r.right && my >= r.top && my <= r.bottom)
		return TRUE;
	return FALSE;
}