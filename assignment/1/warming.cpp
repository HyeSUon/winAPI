#include <iostream>


void menu()
{
	std::cout << "-------------------------------�޴���-----------------------------------" << std::endl;
	std::cout << "= e/E: �ܾ e �Ǵ� E�� ���� ��� �� �ܾ ��� �빮�ڷ� �ٲٰ� ��� =" << std::endl;
	std::cout << "= f/F: ��� ���鿡 .�� �ְ� ���                                       =" << std::endl;
	std::cout << "= l/L: �ܾ��� ������ ���                                              =" << std::endl;
	std::cout << "= c/C: �빮�ڴ� �ҹ��ڷ�, �ҹ��ڴ� �빮�ڷ� �ٲٱ�                     =" << std::endl;
	std::cout << "= a/A: ������ ���ڸ� ����/�������� �� ���ھ� �̵��Ѵ�.                 =" << std::endl;
	std::cout << "= q/Q: ���α׷��� ����                                                 =" << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
}

void pull(char(&ref)[61], int begin) {
	for (int i = begin; ref[i] != NULL; ++i)
		ref[i] = ref[i + 1];
}

bool isSpace(const char& ch)
{
	if (ch == ' ')
		return true;
	return false;
}
bool isAlphabet(const char& ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return true;
	else if (ch >= 'a' && ch <= 'z')
		return true;
	return false;
}

void fFunc(char (&str) [61], bool& fSwit) {

	if (!fSwit) {
		for (int i = 0; str[i] != NULL; ++i) {
			if (str[i] == ' ') {
				str[i] = '.';
			}
		}
		fSwit = true;
	}
	else {
		for (int i = 0; str[i] != NULL; ++i) {
			if (str[i] == '.') {
				str[i] = ' ';
			}
		}
		fSwit = false;
	}
	std::cout << str << std::endl;
}

void lFunc(char(&str)[61])
{
	bool counting = false;
	int count = 0;
	for (int i = 0; str[i] != NULL; ++i) {
		if (!counting) {
			if (isAlphabet(str[i])) {
				counting = true;
				continue;
			}
		}
		if (counting) {
			if (isSpace(str[i])) {
				count += 1;
				counting = false;
			}
		}
	}
	if (counting)
		++count;
	std::cout << count << std::endl;
}
void eFunc(char(&str)[61])
{
	bool counting = false;
	bool isE = false;
	int begin;
	int end;


	for (int i = 0; str[i] != NULL; ++i) {
		if (!counting) {
			if (isE) {
				for (int j = begin; j < end; ++j) {
					if (str[j] >= 'a' && str[j] <= 'z')
						str[j] -= 32;
				}
				isE = false;
			}

			if (isAlphabet(str[i])) {
				counting = true;
				begin = i;
				continue;
			}
		}
		if (counting) {
			
			if ( (str[i] == 'e') || (str[i] == 'E') )
				isE = true;

			if (isSpace(str[i])) {
				counting = false;
				end = i;
			}
		}
	}
	if (isE) {
		for (int i = begin; str[i] != NULL; ++i) {
			if (str[i] >= 'a' && str[i] <= 'z')
				str[i] -= 32;
		}
	}
	std::cout << str << std::endl;
}
void cFunc(char(&str)[61])
{
	for (int i = 0; str[i] != NULL; ++i) {
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		else if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
	std::cout << str << std::endl;
}

void aFunc(char(&str)[61])
{
	int size = 0;
	for (int i = 0; str[i] != NULL; ++i)
		++size;
	
	char temp = str[0];
	for (int i = 0; i < size - 1; ++i)
		str[i] = str[i + 1];
	str[size - 1] = temp;

	std::cout << str << std::endl;
}
int main()
{
	char str[61];
	std::cin.getline(str, 61, '\n');
	for (int i = 0; str[i] != NULL; ++i) {
		if (str[i] == ' ' && str[i + 1] == ' ') {
			pull(str, i--);
		}
	}
	std::cout << str << std::endl;

	char in;
	bool fSwit = false;

	menu();
	while (1) {
		std::cin >> in;
		if (in >= 65 && in <= 90) { // low alphabet
			in += 32;
		}
		switch (in) {
		case 'e':
			eFunc(str);
			break;
		case 'f':
			fFunc(str, fSwit);
			break;
		case 'l':
			lFunc(str);
			break;
		case 'c':
			cFunc(str);
			break;
		case 'a':
			aFunc(str);
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