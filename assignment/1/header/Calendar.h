#pragma once
#include <iostream>
#include <iomanip>
enum class dayOfWeek : int;
enum class month : int;
std::ostream& operator<<(std::ostream&, const dayOfWeek&);
class Calendar;

class Calendar {
	int y, m, d;
	bool leap;
	dayOfWeek thisWeek;
public:
	Calendar(int a, int b, int c);
	void printThisMonth();
	dayOfWeek getThisWeek(const int&, const int&, const int&);
	bool leap_year(const int& year);
	void printFri13th();
};

int dayOfMonth(month);