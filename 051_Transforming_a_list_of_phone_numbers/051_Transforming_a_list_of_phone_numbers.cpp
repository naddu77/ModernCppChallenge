// 051_Transforming_a_list_of_phone_numbers.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std::string_literals;

int main()
{
	std::vector<std::string> numbers{
	  "07555 123456",
	  "07555123456",
	  "+44 7555 123456",
	  "44 7555 123456",
	  "7555 123456"
	};

	auto normalize_phone_numbers = [](auto const& contry_code) {
		return ranges::view::transform([contry_code](auto const& e) {
			if (e.front() == '0')
				return "+"s + contry_code + e.substr(1);
			else if (e.find(contry_code) == 0)
				return "+"s + e;
			else if (e.find("+"s + contry_code) == 0)
				return e;
			else
				return "+"s + contry_code + e;
		});
	};

	auto result = numbers
		| normalize_phone_numbers("44")
		| ranges::view::transform([](auto rng) {
			return rng | ranges::move | ranges::action::remove_if([](auto const c) { return std::isspace(c); });
		})
		;

	std::cout << result << std::endl;

	return 0;
}