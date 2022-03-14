#define _CRT_SECURE_NO_WARNINGS

#include "MemberManager.h"
using namespace std;

int main()
{
	MemberManager mm;
	mm.init();

	while (1) {
		char in;
		cin >> in;
		if (in > 'A' && in < 'Z')
			in -= 'a' - 'A';
		switch (in) {
		case 'a':
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			mm.inputInsert();
			break;
		case 'p':
			mm.printMembers();
			break;
		case 'm':
			mm.maxAtdMb();
			break;
		case 'n':
			mm.minAtdMb();
			break;
		case 'i':
			mm.sortName();
			break;
		case 'j':
			mm.sortNumber();
			break;
		case 'k':
			mm.sortBirthday();
			break;
		case 'q':
			return 0;
			break;
		default:
			if (in >= '1' && in <= '6') {
				int month = in - '0';
				mm.maxThisMount(month);
			}
			else {
				cout << "입력값이 올바르지 않습니다." << endl;
			}
			break;
		}
	}
	
}