// 042_Day_and_week_of_the_year.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::date_time library 이용

#include "pch.h"
#include <boost/date_time.hpp>
#include <iostream>

using namespace boost::gregorian;

int main()
{
	int year;
	int month;
	int day;

	std::cout << "[Year] [Month] [Day]: ";
	std::cin >> year >> month >> day;

	date d(year, month, day);

	std::cout << d.day_of_year() << std::endl;
	std::cout << d.week_number() << std::endl;

	return 0;
}