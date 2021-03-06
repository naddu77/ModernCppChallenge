// 031_Transforming_dates_in_strings.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <cassert>

std::string TransformDate(std::string_view text)
{
	auto rx = std::regex{ R"((\d{1,2})(\.|-|/)(\d{1,2})(\.|-|/)(\d{4}))" };

	return std::regex_replace(text.data(), rx, R"($5-$3-$1)");
}

int main()
{
	using namespace std::string_literals;

	assert(TransformDate("today is 01.12.2017!"s) ==
		"today is 2017-12-01!"s);
}