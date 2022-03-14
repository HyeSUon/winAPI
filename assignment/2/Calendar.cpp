#include "Calendar.h"

Calendar::Calendar(int a, int b, int c) : y{ a }, m{ b }, d{ c }
{
	leap = leap_year(y);
	thisWeek = getThisWeek(y, m, d);
	std::cout << y << '-' << m << '-' << d << ": " << thisWeek << std::endl;
}

void Calendar::printThisMonth()
{
	dayOfWeek this1DayWeek = getThisWeek(y, m, 1);

	std::cout << "  ";
	for (int i = 0; i < 7; ++i) {
		std::cout << std::setw(10) << static_cast<dayOfWeek>(i);
	}
	std::cout << std::endl;
	for (int i = 0; i < static_cast<int>(this1DayWeek); ++i)
		std::cout << std::setw(10) << " ";

	int month_max = dayOfMonth(static_cast<month>(m));
	if (m == 2)
		month_max += leap;

	for (int i = 1; i <= month_max; ++i) {
		std::cout << std::setw(10) << i;
		if ((i + static_cast<int>(this1DayWeek)) % 7  == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

dayOfWeek Calendar::getThisWeek(const int& y, const int& m, const int& d)
{
	int sum = 0;
	for (int year = y - 1; year >= 2020; --year) {
		if (leap_year(year))
			sum += 366;
		else
			sum += 365;
	}
	switch (m - 1) {
	case 11:
		sum += 30;
	case 10:
		sum += 31;
	case 9:
		sum += 30;
	case 8:
		sum += 31;
	case 7:
		sum += 31;
	case 6:
		sum += 30;
	case 5:
		sum += 31;
	case 4:
		sum += 30;
	case 3:
		sum += 31;
	case 2:
		if (leap_year(y))
			sum += 29;
		else
			sum += 28;
	case 1:
		sum += 31;
	}
	sum += d;
	return static_cast<dayOfWeek>((sum + 2) % 7);
}
bool Calendar::leap_year(const int& year)
{
	if (year % 4 == 0) {
		if (year % 400 == 0)
			return true;
		else if (year % 100 != 0)
			return true;
	}
	return false;
}
void Calendar::printFri13th()
{
	for (int year = 2020; year <= 2030; ++year) {
		for (int month = 1; month <= 12; ++month) {
			if (static_cast<int>(getThisWeek(year, month, 13)) == 5)
				std::cout << year << "-" << month << "-" << 13 << std::endl;
		}
	}
}


enum class month : int {
	Jan = 1,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec,
};
int dayOfMonth(month m)
{
	switch (m) {
	case month::Jan:
		return 31;
	case month::Feb:
		return 28;
	case month::Mar:
		return 31;
	case month::Apr:
		return 30;
	case month::May:
		return 31;
	case month::Jun:
		return 30;
	case month::Jul:
		return 31;
	case month::Aug:
		return 31;
	case month::Sep:
		return 30;
	case month::Oct:
		return 31;
	case month::Nov:
		return 30;
	case month::Dec:
		return 31;
	}
}

enum class dayOfWeek : int {
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
};
std::ostream& operator<<(std::ostream& os, const dayOfWeek& week)
{
	switch (week) {
	case dayOfWeek::Sunday:
		os << "Sunday";
		break;
	case dayOfWeek::Monday:
		os << "Monday";
		break;
	case dayOfWeek::Tuesday:
		os << "Tuesday";
		break;
	case dayOfWeek::Wednesday:
		os << "Wednesday";
		break;
	case dayOfWeek::Thursday:
		os << "Thursday";
		break;
	case dayOfWeek::Friday:
		os << "Friday";
		break;
	case dayOfWeek::Saturday:
		os << "Saturday";
		break;
	}
	return os;
}