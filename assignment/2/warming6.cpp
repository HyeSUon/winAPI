#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef void (*funcPtr[6])(const int&);


void shapeX(const int&);
void invertedTriangle(const int&);
void rhombus(const int&);
void butterfly(const int&);
void hourglass(const int&);
void square(const int&);




int main()
{
	funcPtr fp { shapeX, invertedTriangle, rhombus, butterfly, hourglass, square };
	while (1) {
		stringstream ss;
		string str;
		getline(cin, str);
		ss << str;
		int shape, size;
		ss >> shape >> size;

		if (ss.fail()) {
			cout << "숫자 외 입력: 프로그램 종료" << endl;
			break;
		}
		if (shape > 6 || shape < 1 || size < 0 || size > 40) {
			cout << "값의 범위가 올바르지 않습니다. (shape:1~6 size:1~40)" << endl;
			continue;
		}
		fp[shape-1](size);
	}

}

void shapeX(const int& w)
{
	if (w > 10) {
		int h = w / 2;
		bool center = false;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				if ((i * 2 + center) == j || 2 * i + j + center == w - 1)
					cout << '*';
				else
					cout << ' ';
				if (i == (h + 1) / 2)
					center = true;
			}
			cout << endl << endl;
		}
	}
	else {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				if (i == j || i + j == w - 1)
					cout << '*';
				else
					cout << ' ';
			}
			cout << endl;
		}
	}
}

void invertedTriangle(const int& w)
{
	if (w > 10) {
		int h = w / 2;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - i)
					cout << '*';
			}
			cout << endl << endl;
		}
	}
	else {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - i)
					cout << '*';
			}
			cout << endl;
		}
	}
}
void rhombus(const int& w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = (h - 1) / 2; i > 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					cout << ' ';
				else if (j < w - 2 * i)
					cout << '*';
			}
			cout << endl << endl;
		}



		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					cout << ' ';
				else if (j < w - 2 * i)
					cout << '*';
			}
			cout << endl << endl;
		}
	}
	else {
		for (int i = (w - 1); i > 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - i)
					cout << '*';
			}
			cout << endl;
		}



		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - 1 * i)
					cout << '*';
			}
			cout << endl;
		}
	}
}

void butterfly(const int& w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i + 1)
					cout << '*';
				else if (j < w - 2 * i - 1)
					cout << ' ';
			}
			for (int j = 0; j < w; ++j)
				if (j < 2 * i + 1)
					cout << '*';
			cout << endl << endl;
		}
		for (int i = (h - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i + 1)
					cout << '*';
				else if (j < w - 2 * i - 1)
					cout << ' ';
			}
			for (int j = 0; j < w; ++j)
				if (j < 2 * i + 1)
					cout << '*';
			cout << endl << endl;
		}
	}
	else {
		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i + 1)
					cout << '*';
				else if (j < w - i - 1)
					cout << ' ';
			}
			for (int j = 0; j < w; ++j)
				if (j < i + 1)
					cout << '*';
			cout << endl;
		}
		for (int i = (w - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j <  i + 1)
					cout << '*';
				else if (j < w - i - 1)
					cout << ' ';
			}
			for (int j = 0; j < w; ++j)
				if (j < i + 1)
					cout << '*';
			cout << endl;
		}
	}
}
void hourglass(const int& w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					cout << ' ';
				else if (j < w - 2 * i)
					cout << '*';
			}
			cout << endl << endl;
		}
		for (int i = (h - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					cout << ' ';
				else if (j < w - 2 * i)
					cout << '*';
			}
			cout << endl << endl;
		}
	}
	else {
		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - i)
					cout << '*';
			}
			cout << endl;
		}
		for (int i = (w - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					cout << ' ';
				else if (j < w - i)
					cout << '*';
			}
			cout << endl;
		}
	}
}

void square(const int& w)
{
	int h = w / 2;
	for (int i = 0; i < w; ++i)
		cout << '*';
	cout << endl;

	for (int i = 0; i < h - 2; ++i) {
		for (int j = 0; j < w; ++j) {
			if (j == 0 || j == w - 1)
				cout << '*';
			else
				cout << ' ';
		}
		cout << endl;
	}



	for (int i = 0; i < w; ++i)
		cout << '*';
	cout << endl;

}