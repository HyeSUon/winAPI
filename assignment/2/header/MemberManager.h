#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <regex>
#include <algorithm>

struct Data {
	char* name;
	int number;
	int birthday;
	int atd[6];
};

class MemberManager {
private:
	int max_count;
	int cur_count;
	Data* mp_data;
public:
	MemberManager(int a_max_count = 15);
	~MemberManager();

	void init();
	void insert(const char*, int&, int&, int[6]);
	void inputInsert();
	void printMember(int&);
	void printMembers();
	void maxAtdMb();
	void minAtdMb();
	void maxThisMount(int&);
	void sortName();
	void sortNumber();
	void sortBirthday();
};

