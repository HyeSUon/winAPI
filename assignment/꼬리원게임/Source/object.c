#include "object.h"



void InitObject(ObjectManager* om)
{
	srand(time(NULL));
	om->p.d = BOTTOM;
	om->p.speed = 150;
	om->p.color = RGB(255, 255, 0);
	om->objectMaxSize = OBJECT_SIZE;

}

void PushObject(HWND hWnd, ObjectManager* om)
{
	if (om->size == om->objectMaxSize) {
		KillTimer(hWnd, 1);
		return;
	}
	om->obj[om->size].col = rand() % COUNT;
	om->obj[om->size].row = rand() % COUNT;
	if (rand() % 2) {
		om->obj[om->size].isUp = TRUE; // static �����̹Ƿ� default FALSE
	}
	om->obj[om->size].d = rand() % 4;
	om->obj[om->size].movePattern = rand() % 3;
	om->obj[om->size++].cnt = 0;
}

void DeleteTail(ObjectManager* om, int i)
{
	if (om->obj[i].parentID != NULL) {
		if (om->obj[i].childID != NULL) {
			om->obj[i].childID->parentID = om->obj[i].parentID;
			om->obj[i].parentID->childID = om->obj[i].childID;
		}
		else {
			om->obj[i].parentID->childID = NULL;
		}
	}
	else if (om->obj[i].childID != NULL) {
		om->obj[i].childID->parentID = NULL;
	}
	memset(&om->obj[i], 0, sizeof(Object));
	om->size--;
}
void DrawObject(HDC hDC, const ObjectManager* om)
{
	// Block Draw
	HBRUSH hBrush = CreateSolidBrush(RGB(255,0, 255));
	SelectObject(hDC, hBrush);
	for (int i = 0; i < om->blockCnt; ++i) {
		Rectangle(hDC, OBPOS(om->b[i].row, om->b[i].col));
	}
	DeleteObject(hBrush);

	// Object Draw
	hBrush = CreateSolidBrush(RGB(0, 200, 200));
	SelectObject(hDC, hBrush);
	for (int i = 0; i < om->size; ++i) {
		Ellipse(hDC, OBPOS(om->obj[i].row, om->obj[i].col));
	}
	DeleteObject(hBrush);

	// player ������ Draw
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hDC, hBrush);

	Object* p = om->p.childID;
	while (p != NULL) {
		if (om->p.isRectangle)
			Rectangle(hDC, OBPOS(p->row, p->col));
		else
			Ellipse(hDC, OBPOS(p->row, p->col));
		p = p->childID;
	}
	DeleteObject(hBrush);

	// Player Draw
	hBrush = CreateSolidBrush(om->p.color);
	SelectObject(hDC, hBrush);
	if (om->p.isRectangle)
		Rectangle(hDC, OBPOS(om->p.row, om->p.col));
	else
		Ellipse(hDC, OBPOS(om->p.row, om->p.col));
	DeleteObject(hBrush);
}

void UpdateObject(ObjectManager* om)
{
	for (int i = 0; i < om->size - 1; ++i) {
		if (om->obj[i].parentID == NULL) {
			for (int j = i + 1; j < om->size; ++j) {
				if (om->obj[i].col == om->obj[j].col && om->obj[i].row == om->obj[j].row && om->obj[j].parentID == NULL) {
					if (om->obj[i].d != M3) { // ������ �ִ� �������� �������� ���� �ʵ���
						Object* p = &om->obj[i];
						while (p->childID != NULL) {
							p = p->childID;
						}
						p->childID = &om->obj[j];
						om->obj[j].parentID = p;
					}
					else {
						Object* p = &om->obj[j];
						while (p->childID != NULL) {
							p = p->childID;
						}
						p->childID = &om->obj[i];
						om->obj[i].parentID = p;
					}
				}
			}
		}
	}
}
BOOL isObject(ObjectManager* om, int col, int row)
{
	for (int i = 0; i < om->size; ++i) {
		if (om->obj[i].col == col && om->obj[i].row == row)
			return TRUE;
	}
	if (om->p.col == col && om->p.row == row)
		return TRUE;
	for (int i = 0; i < om->blockCnt; ++i) {
		if (om->b[i].col == col && om->b[i].row == row)
			return TRUE;
	}
	return FALSE;
}
void PushBlock(ObjectManager* om)
{
	
	// �� ��ġ�� ������ �ִ��� �˻�
	if (om->blockCnt >= BLOCK_MAX_SIZE)
		return;
	if(isObject(om, om->mCol, om->mRow))
		return;
	om->b[om->blockCnt].col = om->mCol;
	om->b[om->blockCnt++].row = om->mRow;

}
BOOL isBlock(ObjectManager*om, int col, int row)
{
	for (int i = 0; i < om->blockCnt; ++i) {
		if (om->b[i].col == col && om->b[i].row == row)
			return TRUE;
	}
	return FALSE;
}

void ResetGame(ObjectManager* om)
{
	memset(om, 0, sizeof(ObjectManager));
}
void ClickUpdate(HWND hWnd, ObjectManager* om)
{
	if (om->mCol == om->p.col && om->mRow == om->p.row) { // �ӵ� ����
		if(om->p.speed > 50)
			SetTimer(hWnd, 2, om->p.speed - (om->p.speed/50)*40, NULL);
		SetTimer(hWnd, 4, 1000, NULL);
		return;
	}

	// ���ΰ����� ���ӵ� ������ �и�
	Object* p = om->p.childID;
	BOOL isClickObject = FALSE;
	while (p != NULL) {
		if (p->col == om->mCol && p->row == om->mRow) {
			if (om->p.childID == p)
				om->p.childID = NULL;
			else
				p->parentID->childID = NULL;
			isClickObject = TRUE;
		}
		if (isClickObject) {
			if (rand() % 2) {
				p->isUp = TRUE; // static �����̹Ƿ� default FALSE
			}
			p->d = rand() % 4;
			p->movePattern = rand() % 3;
			p->cnt = 0;
			p->parentID = NULL;
			if (p->childID != NULL) {
				p = p->childID;
				p->parentID->childID = NULL;
			}
			else {
				p->childID = NULL;
				break;
			}
		}
		else
			p = p->childID;
	}
	if (isClickObject)
		return;

	// ���콺������ ������ ���ΰ��� �̵�
	if (om->p.d == TOP || om->p.d == BOTTOM) {
		if (om->mCol == om->p.col) {
			if (om->mRow > om->p.row)
				om->p.d = BOTTOM;
			else if (om->mRow < om->p.row)
				om->p.d = TOP;
		}
		else {
			if (om->mCol > om->p.col)
				om->p.d = RIGHT;
			else if (om->mCol < om->p.col)
				om->p.d = LEFT;
		}
	}
	else {
		if (om->mRow == om->p.row) {
			if (om->mCol > om->p.col)
				om->p.d = RIGHT;
			else if (om->mCol < om->p.col)
				om->p.d = LEFT;
		}
		else {
			if (om->mRow > om->p.row)
				om->p.d = BOTTOM;
			else if (om->mRow < om->p.row)
				om->p.d = TOP;
		}
	}

}
void UpdatePlayer(ObjectManager* om)
{
	// �÷��̾ ���ӵ� ������
	for (int i = 0; i < om->size; ++i) {
		if (om->obj[i].col == om->p.col && om->obj[i].row == om->p.row && om->obj[i].parentID == NULL) {
			if (om->p.childID == NULL) {
				om->p.childID = &om->obj[i];
				om->obj[i].parentID = &om->p;
			}
			else {
				Object* p = om->p.childID;
				while (p->childID != NULL) {
					p = p->childID;
				}
				p->childID = &om->obj[i];
				om->obj[i].parentID = p;
			}
		}
	}
}
void MovePlayer(HWND hWnd, ObjectManager* om)
{
	om->p.old = (Point){ om->p.col, om->p.row };
	// Player Move
	if (om->p.d == LEFT) {
		om->p.col--;
		if (om->p.col < 0) {
			om->p.col++;
			om->p.isUp ? om->p.row-- : om->p.row++;

			if (om->p.row < 0) {
				om->p.isUp = !om->p.isUp;
				om->p.row = 1;
			}
			if (om->p.row >= COUNT) {
				om->p.isUp = !om->p.isUp;
				om->p.row = COUNT - 2;
			}
			om->p.d = RIGHT;
		}
	}
	else if (om->p.d == RIGHT) {
		om->p.col++;
		if (om->p.col >= COUNT) {
			om->p.col--;
			om->p.isUp ? om->p.row-- : om->p.row++;
			if (om->p.row < 0) {
				om->p.isUp = !om->p.isUp;
				om->p.row = 1;
			}
			if (om->p.row >= COUNT) {
				om->p.isUp = !om->p.isUp;
				om->p.row = COUNT - 2;
			}
			om->p.d = LEFT;
		}
	}
	else if (om->p.d == TOP) {
		om->p.row--;
		if (om->p.row < 0) {
			om->p.row++;
			om->p.isUp ? om->p.col-- : om->p.col++;

			if (om->p.col < 0) {
				om->p.isUp = !om->p.isUp;
				om->p.col = 1;
			}
			if (om->p.col >= COUNT) {
				om->p.isUp = !om->p.isUp;
				om->p.col = COUNT - 2;
			}
			om->p.d = BOTTOM;
		}
	}
	else if (om->p.d == BOTTOM) {
		om->p.row++;
		if (om->p.row >= COUNT) {
			om->p.row--;
			om->p.isUp ? om->p.col-- : om->p.col++;
			if (om->p.col < 0) {
				om->p.isUp = !om->p.isUp;
				om->p.col = 1;
			}
			if (om->p.col >= COUNT) {
				om->p.isUp = !om->p.isUp;
				om->p.col = COUNT - 2;
			}
			om->p.d = TOP;
		}
	}
	Player* p = &om->p;

	for (Object* p = om->p.childID; p != NULL; p = p->childID) {
		if (p->parentID == &om->p) {
			p->old = (Point){ p->col, p->row };
			p->col = om->p.old.col;
			p->row = om->p.old.row;
		}
		else {
			p->old = (Point){ p->col, p->row };
			p->col = p->parentID->old.col;
			p->row = p->parentID->old.row;
		}
	}

	if (isBlock(om, om->p.col, om->p.row)) {
		om->p.col = om->p.old.col;
		om->p.row = om->p.old.row;
		SetTimer(hWnd, 5, om->p.speed, NULL);
	}
}

void MoveObject(ObjectManager* om)
{
	for (int i = 0; i < om->size; ++i) {

		if (om->obj[i].parentID == NULL) {
			om->obj[i].old.col = om->obj[i].col;
			om->obj[i].old.row = om->obj[i].row;

			switch (om->obj[i].movePattern) {
			case M1: // �¿� �͸�����
				if (om->obj[i].d == LEFT) {
					om->obj[i].col--;
					if (om->obj[i].col < 0) {
						om->obj[i].col++;
						om->obj[i].d = RIGHT;
					}
				}
				else if (om->obj[i].d == RIGHT) {
					om->obj[i].col++;
					if (om->obj[i].col >= COUNT) {
						om->obj[i].col--;
						om->obj[i].d = LEFT;
					}
				}
				else if (om->obj[i].d == BOTTOM) {
					om->obj[i].row++;
					if (om->obj[i].row >= COUNT) {
						om->obj[i].row--;
						om->obj[i].d = TOP;
					}
				}
				else if (om->obj[i].d == TOP) {
					om->obj[i].row--;
					if (om->obj[i].row < 0) {
						om->obj[i].row++;
						om->obj[i].d = BOTTOM;
					}
				}
				break;
			case M2: // �׸� �׸��鼭 �̵�
				if (om->obj[i].d == LEFT) {
					om->obj[i].col--;
					if (++om->obj[i].cnt >= 5 || om->obj[i].col < 0) {
						om->obj[i].col++;
						om->obj[i].d = BOTTOM;
					}
				}
				else if (om->obj[i].d == RIGHT) {
					om->obj[i].col++;
					if (++om->obj[i].cnt >= 5 || om->obj[i].col >= COUNT) {
						om->obj[i].col--;
						om->obj[i].d = TOP;
					}
				}
				else if (om->obj[i].d == BOTTOM) {
					om->obj[i].row++;
					if (--om->obj[i].cnt <= 0 || om->obj[i].row >= COUNT) {
						om->obj[i].row--;
						om->obj[i].d = RIGHT;
					}
				}
				else if (om->obj[i].d == TOP) {
					om->obj[i].row--;
					if (--om->obj[i].cnt <= 0 || om->obj[i].row < 0) {
						om->obj[i].row++;
						om->obj[i].d = LEFT;
					}
				}
				break;
			case M3: // ������ �ֱ�
				break;
			}
			// ���ӵ� object move
			if (om->obj[i].parentID == NULL && om->obj[i].childID != NULL) {
				for (Object* p = om->obj[i].childID; p != NULL; p = p->childID) {
					p->old = (Point){ p->col, p->row };
					p->col = p->parentID->old.col;
					p->row = p->parentID->old.row;
				}
			}
		}
	}
}