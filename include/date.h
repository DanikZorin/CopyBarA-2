#ifndef DATE_
#define DATE_

#include <string>
#include <iostream>
class Date{
	static std::string digits;
	int year;
	int month;
	int day;

	void convert(const std::string& sDate);
public:
	Date();
	Date(const std::string& date);

	static int compare(const Date& a, const Date& b);

	std::string toString() const;

	friend std::istream& operator>>(std::istream& stream, Date& date);
	friend std::ostream& operator<<(std::ostream& stream, Date& date);
};

#endif
