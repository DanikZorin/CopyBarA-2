#include "../include/date.h"
std::string Date::digits = "1234567890";
void Date::convert(const std::string& sDate){
	//####-##-##
	
	int pos1=0, pos2;

	int* dat[] = {&year, &month, &day};

	for (int i = 0;i < 3;i++) {
		pos1 = sDate.find_first_of(digits, pos1);
		if (pos1 == std::string::npos) {
			throw 1;
		}
		pos2 = sDate.find_first_not_of(digits, pos1);

		*(dat[i]) = std::stoi(sDate.substr(pos1, pos2-pos1));

		pos1 = pos2;
	}
	if (year < 0) year = 0;
	if (year > 9999) year = 9999;
	if (month < 0) month = 0;
	if (month > 12) month = 12;
	if (day < 0) day = 0;
	if (day > 31) day = 31;
}

Date::Date(){
	year = 0;
	month = 0;
	day = 0;
}

Date::Date(const std::string& date){
	convert(date);
}

int Date::compare(const Date& a, const Date& b)
{
	int timeA = 12*31 * a.year + 31 * (a.month-1)+ a.day;
	int timeB = 12*31 * b.year + 31 * (b.month-1) + b.day;
	if (timeA > timeB) {
		return 1;
	}
	if (timeA < timeB) {
		return -1;
	}
	return 0;
}

std::string Date::toString() const {
	return (year < 10 ? "0" : "") + std::string(year < 100 ? "0" : "") + (year < 1000 ? "0" : "") + std::to_string(year) + "-" + (month<10 ? "0" : "") + std::to_string(month) + "-" + (day < 10 ? "0" : "") + std::to_string(day);
}

std::istream& operator>>(std::istream& stream, Date& date){
	std::string sDate;
	std::getline(stream, sDate);
	date.convert(sDate);
	return stream;
}
std::ostream& operator<<(std::ostream& stream, Date& date) {
	stream << date.toString();
	return stream;
}
