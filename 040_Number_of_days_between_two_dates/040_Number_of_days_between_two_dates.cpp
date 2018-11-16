// 040_Number_of_days_between_two_dates.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::date_time library 이용

#include "pch.h"
#include <boost/date_time.hpp>
#include <iostream>
#include <algorithm>

using namespace boost::gregorian;

int main()
{
	int year1;
	int month1;
	int day1;
	int year2;
	int month2;
	int day2;

	std::cout << "[Year] [Month] [Day]: ";
	std::cin >> year1 >> month1 >> day1;
	std::cout << "[Year] [Month] [Day]: ";
	std::cin >> year2 >> month2 >> day2;

	auto result = std::minmax(date(year1, month1, day1), date(year2, month2, day2));

	std::cout << "difference: " << result.second - result.first << " days." << std::endl;

	return 0;
}
