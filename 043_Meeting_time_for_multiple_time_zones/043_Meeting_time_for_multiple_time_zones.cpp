// 043_Meeting_time_for_multiple_time_zones.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::date_time library 이용

#include "pch.h"
#include <boost/date_time.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <sstream>

int main()
{
	using namespace boost::local_time;
	using namespace boost::posix_time;

	tz_database tz_db;
	tz_db.load_from_file("data/date_time_zonespec.csv");

	using User = std::tuple<std::string, std::string>;

	std::vector<User> users{
	  { "Ildiko", "Europe/Budapest" },
	  { "Jens", "Europe/Berlin" },
	  { "Jane", "America/New_York" },
	  { "Yoo", "Asia/Seoul" }
	};

	std::size_t h;
	std::size_t m;

	std::cout << "Hour:";
	std::cin >> h;
	std::cout << "Minutes:"; 
	std::cin >> m;

	local_date_time meeting_time(second_clock::local_time().date(), time_duration(h, m, 0), tz_db.time_zone_from_region("Asia/Seoul"), local_date_time::NOT_DATE_TIME_ON_ERROR);
	std::stringstream ss;

	ss << std::left << std::setw(15) << std::setfill(' ')
		<< "Local time: " << meeting_time;

	std::vector meeting_time_list(1, ss.str());

	std::transform(std::cbegin(users), std::cend(users), std::back_inserter(meeting_time_list), [tz_db, meeting_time](auto const& user) {
		std::stringstream ss;

		ss << std::left << std::setw(15) << std::setfill(' ')
			<< std::get<0>(user)
			<< meeting_time.local_time_in(tz_db.time_zone_from_region(std::get<1>(user)));

		return ss.str();
	});

	std::copy(std::cbegin(meeting_time_list), std::cend(meeting_time_list), std::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}
