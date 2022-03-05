#include <iostream>
#include <conio.h>
#include <random>
#include <windows.h>
char pos[10][10];

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<> rand09(0, 9);
std::uniform_int_distribution<> rand04(0, 4);

struct Doll{
	int x, y;
	char shape;
	bool behind = false;
	Doll(int a, int b, char shape) : x{ a }, y{ b }, shape{ shape } { }
	
};

void clear();
void printBoard(const Doll&, const Doll&, bool);
void init(const Doll&, const Doll&, bool);
void update(Doll&, Doll&, int&, bool&);
void posUpdate(const Doll&, const Doll&);
void randShape(Doll&);
void resetDoll(Doll&, Doll&);
int main()
{
	Doll p1(rand09(dre), rand09(dre), 'X');
	Doll p2(rand09(dre), rand09(dre), 'O');
	bool p1Turn = true;

	init(p1, p2, p1Turn);
	while (1) {

		int key_value;

		key_value = _getch();
		if (key_value == 0x00 || key_value == 0xE0) {
			key_value = _getch(); // 확장키의 경우 예외 처리
		}
		if (key_value)
		{	
			//update
			update(p1, p2, key_value, p1Turn);
			posUpdate(p1, p2);
			//draw
			clear();
			printBoard(p1, p2, p1Turn);
		}
			
	}
}

void randShape(Doll& player)
{
	int shape = rand04(dre);
	switch (shape) { // 0: O, 1: X, 2: #, 3: @, 4: *
	case 0:
		player.shape = 'O';
		break;
	case 1:
		player.shape = 'X';
		break;
	case 2:
		player.shape = '#';
		break;
	case 3:
		player.shape = '@';
		break;
	case 4:
		player.shape = '*';
		break;
	default:
		std::cerr << "randShape예외발생";
	}
}
void resetDoll(Doll& p1, Doll& p2)
{
	p1.x = rand09(dre);
	p1.y = rand09(dre);
	p2.x = rand09(dre);
	p2.y = rand09(dre);
	randShape(p1);
	randShape(p2);
}
void clear()
{
	std::cout << "\x1B[2J\x1B[H";
}

void init(const Doll& p1,const Doll& p2, bool p1Turn)
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j)
			pos[i][j] = ' ';
	}
	posUpdate(p1, p2);
	printBoard(p1, p2, p1Turn);
}

bool ExistDoll(int x, int y)
{
	if (pos[x][y] != ' ')
		return true;
	return false;
}
void posUpdate(const Doll& p1, const Doll& p2)
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j)
			pos[i][j] = ' ';
	}
	if (!p1.behind)
		pos[p1.y][p1.x] = p1.shape;
	if (!p2.behind)
		pos[p2.y][p2.x] = p2.shape;
}
void update(Doll& p1, Doll& p2, int& key_value, bool& p1Turn) // 리셋:r
{
	if (p1Turn) {
		switch (key_value) {
		case 'w':
			if (--p1.y < 0) {
				randShape(p1);
				p1.y = 9;
			}
			break;
		case 'a':
			if (--p1.x < 0) {
				p1.x = 9;
				randShape(p1);
			}
			break;
		case 's':
			if (++p1.y / 10)
				randShape(p1);
			p1.y %= 10;
			break;
		case 'd':
			if (++p1.x / 10)
				randShape(p1);
			p1.x %= 10;
			break;
		case 'r':
			resetDoll(p1, p2);
			break;
		case 'q':
			exit(1);
			break;
		default:
			return;
		}
		if (p1.behind) {
			randShape(p1);
			p1.behind = false;
		}
		if (p1.x == p2.x && p1.y == p2.y) {
			Beep(330, 200);
			p2.behind = true;
		}
		p1Turn = false;
	}
	else {
		switch (key_value) {
		case 'i':
			if (--p2.y < 0) {
				randShape(p2);
				p2.y = 9;
			}
			break;
		case 'j':
			if (--p2.x < 0) {
				p2.x = 9;
				randShape(p2);
			}
			break;
		case 'k':
			if (++p2.y / 10)
				randShape(p2);
			p2.y %= 10;
			break;
		case 'l':
			if (++p2.x / 10)
				randShape(p2);
			p2.x %= 10;
			break;
		case 'r':
			resetDoll(p1, p2);
			break;
		case 'q':
			exit(1);
			break;
		default:
			return;
		}
		if (p2.behind) {
			randShape(p2);
			p2.behind = false;
		}
		if (p1.x == p2.x && p1.y == p2.y) {
			Beep(330, 200);
			p1.behind = true;
		}
		p1Turn = true;
	}
}
void printBoard(const Doll& p1, const Doll& p2, bool p1Turn)
{
	std::cout << "---------------------" << std::endl;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			std::cout << '|' << pos[i][j];
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "---------------------" << std::endl;

	if (p1Turn)
		std::cout << "                                   " << "Player1 턴" << std::endl;
	else
		std::cout << "                                   " << "Player2 턴" << std::endl;
	std::cout << "                                   " << "P1 Behind: " << p1.behind << std::endl;
	std::cout << "                                   " << "P2 Behind: " << p2.behind << std::endl;
}