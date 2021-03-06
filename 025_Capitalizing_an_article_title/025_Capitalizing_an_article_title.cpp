// 025_Capitalizing_an_article_title.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: std::transform

#include "pch.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>
#include <cassert>
#include <execution>

auto Capitalize(std::string str)
{
	auto is_first = true;

	std::transform(std::execution::par, std::begin(str), std::end(str), std::begin(str), [&is_first](auto const c) {
		auto result = std::exchange(is_first, false) ? std::toupper(c) : std::tolower(c);

		is_first = std::isspace(c) || std::ispunct(c);

		return result;
	});

	return str;
}

int main()
{
	std::string input = "the c++ challenger";
	

	using namespace std::string_literals;

	assert("The C++ Challenger"s ==
		Capitalize("the c++ challenger"s));
	assert("This Is An Example, Should Work!"s ==
		Capitalize("THIS IS an ExamplE, should wORk!"s));


	return 0;
}
