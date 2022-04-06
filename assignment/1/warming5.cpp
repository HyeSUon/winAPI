#include <iostream>
#include <string>
#include <regex>

using namespace std;

int calculation(const int& , char& ,const int& );
void precedence(string&, int (&)[5]);

int main()
{
	int operand[5];
	string oper;
	while (1) {
		cout << "input numbers: ";
		string str;
		getline(cin, str);
		smatch match;
		regex re("(\\d{1,2})([\*\+\/\-])(\\d{1,2})([\*\+\/\-])(\\d{1,2})([\*\+\/\-])(\\d{1,2})([\*\+\/\-])(\\d{1,2})");
		regex quit("['q''Q']");
		if (regex_match(str, match, re)) {
			for (int i = 1; i < 10; ++i) {
				if (i % 2)
					operand[i / 2] = stoi(match[i].str());
				else
					oper.insert(i / 2 - 1, match[i].str());
			}

		}
		else if (regex_match(str, quit)) {
			return 0;
		}
		else
		{
			continue;
		}

		for (int i = 0; i < 4; ++i)
			precedence(oper, operand);
		for (int i = 0; i < oper.size() + 1; ++i)
			cout << "Result: " << operand[i] << endl;
	}
}

void precedence(string& oper, int (&operand)[5])
{
	for (int i = 0; i < oper.size(); ++i) {
		if (oper[i] == '*' || oper[i] == '/') {
			operand[i] = calculation(operand[i], oper[i], operand[i + 1]);
			oper.erase(i, 1);
			for (int j = i+1; j < oper.size()+1; ++j)
				operand[j] = operand[j+1];
			return;
		}
	}
	for (int i = 0; i < oper.size(); ++i) {
		if (oper[i] == '+' || oper[i] == '-') {
			operand[i] = calculation(operand[i], oper[i], operand[i + 1]);
			oper.erase(i, 1);
			for (int j = i + 1; j < oper.size() + 1; ++j)
				operand[j] = operand[j + 1];
			return;
		}
	}
}

int calculation(const int& a, char& oper,const int& b)
{
	switch (oper) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		return a / b;
	}
}


