// 026_Joining_strings_together_separated_by_a_delimiter.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <numeric>

template <typename Cont>
auto JoinStrings(Cont const& cont, char const * delimeter)
{
	std::stringstream ss;
	auto is_first = true;

	for (auto const& e : cont)
	{
		if (!std::exchange(is_first, false))
			ss << delimeter;

		ss << e;
	}

	return ss.str();
}

int main()
{
	using namespace std::string_literals;

	std::vector<std::string> v1{ "this", "is", "an", "example" };
	std::vector<std::string> v2{ "example" };
	std::vector<std::string> v3{ };

	assert(JoinStrings(v1, " ") == "this is an example"s);
	assert(JoinStrings(v2, " ") == "example"s);
	assert(JoinStrings(v3, " ") == ""s);

	return 0;
}

