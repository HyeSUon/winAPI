#include "Calendar.h"
#include <sstream>
#include <vector>
using namespace std;

vector<string> split(string, char, char, char);
bool leap_year(const int& year);
bool checkScope(const int& y, const int& m, const int& d);

int main()
{
	while (1) {
		cout << "날짜를 입력하세요. ex) yyyy/mm/dd (사용 가능한 구분자: '/', '-', ' ')" << endl;
		string str;
		getline(cin, str);
		vector<string> vs = split(str, ' ', '-', '/');
		if (vs.size() != 3) {
			cout << "올바른 값을 입력해주세요." << endl;
			continue;
		}

		int y, m, d;
		y = stoi(vs[0]);
		m = stoi(vs[1]);
		d = stoi(vs[2]);

		if (!checkScope(y, m, d))
			continue;

		Calendar cal(y, m, d);
		cal.printThisMonth();
		cout << "f/F: 13일의 금요일 출력" << endl;
		cout << "r/R: 날짜 다시 받기" << endl;
		cout << "q: 종료" << endl;
		char in;
		cin >> in;
		cin.ignore();
		switch (in) {
		case 'f':
			cal.printFri13th();
			break;
		case 'F':
			cal.printThisMonth();
			break;
		case 'r':
			continue;
		case 'R':
			continue;
		case 'q':
			exit(1);
		default:
			cout << "f/F: 13일의 금요일 출력" << endl;
			cout << "q: 종료" << endl;
			continue;
		}
	}
}

bool leap_year(const int& year)
{
	if (year % 4 == 0) {
		if (year % 400 == 0)
			return true;
		else if (year % 100 != 0)
			return true;
	}
	return false;
}
bool checkScope(const int& y, const int& m, const int& d)
{
	bool leap = leap_year(y);
	if (y < 2020 || y > 2030) {
		std::cout << "2020~2030년까지만 입력해주세요. " << std::endl;
		return false;
	}
	if (m > 12 || m < 1) {
		std::cout << "비정상적인 날짜입니다. " << std::endl;
		return false;
	}
	if (m == 2) {
		if (d > 28 + leap || d < 0) {
			std::cout << "비정상적인 날짜입니다. " << std::endl;
			return false;
		}
	}
	else {
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {
			if (d > 31 || d < 0) {
				std::cout << "비정상적인 날짜입니다. " << std::endl;
				return false;
			}
		}
		else if (m == 4 || m == 6 || m == 9 || m == 11) {
			if (d > 30 || d < 0) {
				std::cout << "비정상적인 날짜입니다. " << std::endl;
				return false;
			}
		}
		else {
			std::cout << "비정상적인 날짜입니다. " << std::endl;
			return false;
		}
	}
	return true;
}


vector<string> split(string str, char delimiter1 ,char delimiter2, char delimiter3) 
{
	vector<string> v;

	char checkDelimiter[3];
	int cnt = 0;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == delimiter1)
			checkDelimiter[cnt++] = delimiter1;
		else if (str[i] == delimiter2)
			checkDelimiter[cnt++] = delimiter2;
		else if (str[i] == delimiter3)
			checkDelimiter[cnt++] = delimiter3;
	}

	if (cnt != 2)
		return v;

	int start = 0;
	int d = str.find(checkDelimiter[0]);

	int i = 1;
	while (i < 3) {
		v.push_back(str.substr(start, d - start));
		start = d + 1;
		d = str.find(checkDelimiter[i++], start);
	}
	v.push_back(str.substr(start, d - start));
	return v;
}