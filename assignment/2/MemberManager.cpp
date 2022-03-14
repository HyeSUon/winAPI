#include "MemberManager.h"

int dayOfMonth[12]{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

MemberManager::MemberManager(int a_max_count) : max_count{ a_max_count }, cur_count{ 0 }
{
	mp_data = new Data[max_count];
	std::memset(mp_data, 0, sizeof(Data) * max_count);
	std::cout << "핸들 관리자 생성" << std::endl;
}
MemberManager::~MemberManager()
{
	Data* p = mp_data;
	std::cout << "문자열 삭제중";
	for (int i = 0; i < max_count; ++i) {
		if (p->name) {
			delete[] p->name;
			std::cout << ".";
		}
		p++;
	}
	delete[] mp_data;
	std::cout << "삭제 완료" << std::endl;
}
void MemberManager::init()
{
	int n = 100;
	int b = 101;
	int temp[6] = { 12, 11, 10 ,10, 10, 8 };
	this->insert("Kim", n, b, temp);

	n = 101;
	b = 301;
	int temp2[6] = {13, 12, 10 ,20, 10, 13 };
	this->insert("Jin", n, b, temp2);

	n = 102;
	b = 1221;
	int temp3[6] = { 0, 0, 0 ,0, 4, 3 };
	this->insert("Lee", n, b, temp3);
}
void MemberManager::insert(const char* name, int& n, int& b, int at[6])
{
	Data* p = mp_data;
	int i;
	for (i = 0; i < max_count; i++) {
		if (p->name == NULL) {
			break;
		}
		p++;
	}
	if (i < max_count) {
		int len = strlen(name) + 1;
		p->name = new char[len];
		memcpy(p->name, name, len);

		p->number = n;
		p->birthday = b;

		for (int i = 0; i < 6; ++i)
			p->atd[i] = at[i];
		cur_count++;
		return;
	}
	std::cout << "더이상 회원정보를 추가할 수 없습니다." << std::endl;
}

void MemberManager::inputInsert()
{
	std::string str;
	std::getline(std::cin, str);
	std::smatch match;
	Data temp;
	std::string str_temp;
	std::regex re("([a-z]{1,5}) (\\d{3}) (\\d{3,4}) (\\d{1,2}) (\\d{1,2}) (\\d{1,2}) (\\d{1,2}) (\\d{1,2}) (\\d{1,2})", std::regex::icase);
	if (std::regex_match(str, match, re)) {
		str_temp = match[1];
		temp.number = stoi(match[2]);
		temp.birthday = stoi(match[3]);
		for (int i = 0; i < 6; ++i)
			temp.atd[i] = stoi(match[4 + i]);
	}
	else {
		std::cout << "형식이 올바르지 않습니다." << std::endl;
		return;
	}
	Data* p = mp_data;
	if (5 < strlen(str_temp.c_str())) {
		std::cout << "이름: 5자리 이하만 가능합니다." << std::endl;
		return;
	}
	if (temp.number < 1  || temp.number > 999) {
		std::cout << "회원 번호: 3자리 숫자로 입력해주세요" << std::endl;
		return;
	}
	if (temp.birthday / 100 < 0 || temp.birthday / 100 > 12 || temp.birthday % 100 > dayOfMonth[temp.birthday / 100 - 1]) {
		std::cout << "생일: mmdd 형식으로 날짜에 맞게 입력해주세요. " << std::endl;
		return;
	}
	for (int i = 0; i < 6; ++i) {
		if (temp.atd[i] < 0 || temp.atd[i] > dayOfMonth[i]) {
			std::cout << "출석 현황이 허용된 숫자를 벗어납니다." << std::endl;
			return;
		}
	}

	int i;
	for (i = 0; i < max_count; i++) {
		if (p->name == NULL) {
			break;
		}
		p++;
	}
	if (i < max_count) {
		int len = strlen(str_temp.c_str()) + 1;
		p->name = new char[len];
		memcpy(p->name, str_temp.c_str(), len);
		std::cout << p->name << "문자열 추가 완료" << std::endl;

		p->number = temp.number;
		p->birthday = temp.birthday;

		for (int i = 0; i < 6; ++i)
			p->atd[i] = temp.atd[i];
		cur_count++;
		return;
	}
	std::cout << "더이상 회원정보를 추가할 수 없습니다." << std::endl;
}

void MemberManager::printMembers()
{
	Data* p = mp_data;

	std::cout << "+--------------------------------------+" << std::endl;
	for (int i = 0; i < max_count; ++i) {
		if (p->name != NULL) {
			this->printMember(i);
		}
		p++;
	}
	std::cout << "+--------------------------------------+" << std::endl;
}


void MemberManager::printMember(int& n) // 0부터
{
	Data* p = mp_data;
	p += n;
	std::cout << "| " << std::left << std::setfill(' ') << std::setw(2) << n + 1 << ": "
		<< std::setfill('_') << std::setw(5) << p->name << ' ' << std::right <<
		std::setfill('0') << std::setw(3) << p->number << " " << std::setw(4) << p->birthday << " ";
	for (int n : p->atd)
		std::cout << std::setw(2) << n << " ";
	std::cout << '|' << std::endl;
}


void MemberManager::maxAtdMb()
{
	if (cur_count == 0) {
		std::cout << "저장된 회원정보가 없습니다." << std::endl;
		return;
	}
	Data* p = mp_data;
	int* addAtd{ new int[cur_count] {} };

	for (int i = 0; i < cur_count; ++i) {
		if (p->name != NULL) {
			for (int j = 0; j < 6; ++j)
				addAtd[i] += p->atd[j];
		}
		++p;
	}

	int maxAtdN = 0;
	int maxAtd{};
	for (int i = 0; i < cur_count; ++i) {
		if (addAtd[i] > maxAtd) {
			maxAtd = addAtd[i];
			maxAtdN = i;
		}
		++p;
	}
	this->printMember(maxAtdN);

	delete[] addAtd;
}

void MemberManager::minAtdMb()
{
	if (cur_count == 0) {
		std::cout << "저장된 회원정보가 없습니다." << std::endl;
		return;
	}
	Data* p = mp_data;
	int* addAtd{ new int[cur_count] {} };

	for (int i = 0; i < cur_count; ++i) {
		if (p->name != NULL) {
			for (int j = 0; j < 6; ++j)
				addAtd[i] += p->atd[j];
		}
		++p;
	}

	int minAtdN = 0;
	int minAtd{200};
	for (int i = 0; i < cur_count; ++i) {
		if (addAtd[i] < minAtd) {
			minAtd = addAtd[i];
			minAtdN = i;
		}
		++p;
	}
	this->printMember(minAtdN);

	delete[] addAtd;
}

void MemberManager::maxThisMount(int& month)
{
	if (cur_count == 0) {
		std::cout << "저장된 회원정보가 없습니다." << std::endl;
		return;
	}
	Data* p = mp_data;
	int max = 0;
	int maxN;
	for (int i = 0; i < cur_count; ++i) {
		if (max < p->atd[month - 1]) {
			max = p->atd[month - 1];
			maxN = i;
		}
		p++;
	}
	this->printMember(maxN);
}

void MemberManager::sortName()
{
	Data* p = mp_data;
	std::sort(p, p + cur_count, [](Data& a, Data& b) {
		int compare = strcmp(b.name, a.name);
		if (compare == 1)
			return true;
		else if(compare == -1)
			return false;
		});
}

void MemberManager::sortNumber()
{
	Data* p = mp_data;
	std::sort(p, p + cur_count, [](Data& a, Data& b) {
		return a.number < b.number;
		});
}
void MemberManager::sortBirthday()
{
	Data* p = mp_data;
	std::sort(p, p + cur_count, [](Data& a, Data& b) {
		return a.birthday < b.birthday;
		});
}