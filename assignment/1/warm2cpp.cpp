#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>


std::random_device rd;
std::mt19937 dre(rd()); // default_random_engine
std::uniform_int_distribution<int> uid(1, 50);
std::uniform_int_distribution<int> uid2(0, 19);

int num[20];

void menu()
{
	std::cout << "-------------------------------메뉴얼-----------------------------------" << std::endl;
	std::cout << "= a: 1행 1열을 시작으로 행을 다라 오름차순으로 정렬                    =" << std::endl;
	std::cout << "= d: 1행 5열을 시작으로 열을 따라 내림차순으로 정렬                    =" << std::endl;
	std::cout << "= e: 짝수만 출력하기                                                   =" << std::endl;
	std::cout << "= o: 홀수만 출력하기                                                   =" << std::endl;
	std::cout << "= m:최대값 출력                                                        =" << std::endl;
	std::cout << "= n: 최소값 출력                                                       =" << std::endl;
	std::cout << "= p: 한 행의 값을 모두 더해 각 행의 1열에 저장                         =" << std::endl;
	std::cout << "= r: 랜덤하게 재정렬                                                   =" << std::endl;
	std::cout << "= s: 다시 입력받기 시작                                                =" << std::endl;
	std::cout << "= q: 프로그램 종료                                                     =" << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
}


bool isExist( const int& n, const int& endi)
{
	for (int i = 0; i < endi; ++i) {
		if (n == num[i])
			return true;
	}
	return false;
}
void mEnter(int(&m)[4][5])
{
	for (int i = 0; i < 20; ++i) {
		num[i] = uid(dre);
		if (isExist(num[i], i)) {
			--i;
			continue;
		}
	}
	for (int i = 0; i < 20; ++i)
		m[i / 5][i % 5] = num[i];
}

void swap(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}
void printM(int(&m)[4][5])
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j)
			std::cout << std::setw(3) << m[i][j];
		std::cout << std::endl;
	}
}

void aFunc(int(&m)[4][5])
{
	std::sort(num, num + 20, [](int a, int b) {
		return a < b;
		});
	for (int i = 0; i < 20; ++i)
		m[i / 5][i % 5] = num[i];
	printM(m);
}

void dFunc(int(&m)[4][5])
{
	std::sort(num, num + 20, [](int a, int b) {
		return a > b;
		});
	for (int i = 0; i < 20; ++i)
		m[i%4][4 - i/4] = num[i];
	printM(m);
}

void eFunc(int (&m)[4][5])
{
	int temp[4][5];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			temp[i][j] = m[i][j];
			if (temp[i][j] % 2)
				temp[i][j] = 0;
		}
	}
	printM(temp);
}

void oFunc(int(&m)[4][5])
{
	int temp[4][5];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			temp[i][j] = m[i][j];
			if (!(temp[i][j] % 2))
				temp[i][j] = 0;
		}
	}
	printM(temp);
}

void mFunc(int(&m)[4][5])
{
	int max = 0;
	for (int i = 0; i < 20; ++i)
	{
		if (max < num[i])
			max = num[i];
	}
	std::cout << "최대값: " << max << std::endl;
}
void nFunc(int(&m)[4][5])
{
	int min = 51;
	for (int i = 0; i < 20; ++i)
	{
		if (min > num[i])
			min = num[i];
	}
	std::cout << "최소값: " << min << std::endl;
}
void pFunc(int(&m)[4][5])
{
	int temp[4][5];
	for (int i = 0; i < 4; ++i)
		temp[i][0] = m[i][0];
	for (int i = 0; i < 4; ++i) {
		for (int j = 1; j < 5; ++j) {
			temp[i][j] = m[i][j];
			temp[i][0] += m[i][j];
		}
	}
	printM(temp);
}
void rFunc(int(&m)[4][5])
{
	for (int i = 0; i < 20; ++i)
		swap(num[uid2(dre)], num[uid2(dre)]);

	for (int i = 0; i < 20; ++i)
		m[i/5][i%5] = num[i];
	printM(m);
	
}
void sFunc(int(&m)[4][5])
{
	mEnter(m);
	printM(m);
}
int main()
{
	int m[4][5];
	mEnter(m);
	
	char in;
	menu();
	while (1) {
		std::cin >> in;
		switch (in) {
		case 'a':
			aFunc(m);
			break;
		case 'd':
			dFunc(m);
			break;
		case 'e':
			eFunc(m);
			break;
		case 'o':
			oFunc(m);
			break;
		case 'm':
			mFunc(m);
			break;
		case 'n':
			nFunc(m);
			break;
		case 'p':
			pFunc(m);
			break;
		case 'r':
			rFunc(m);
			break;
		case 's':
			sFunc(m);
			break;
		case 'q':
			exit(1);
			break;
		default:
			menu();
			break;
		}
	}
}