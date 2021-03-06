// 027_Splitting_a_string_into_tokens.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: Regular Expression 사용

#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <cassert>
#include <string_view>

auto Split(std::string const& input, std::string const& delimeter)
{
	std::vector<std::string> output;

	std::regex delim(delimeter);

	std::copy(std::sregex_token_iterator(input.begin(), input.end(), delim, -1),
		std::sregex_token_iterator(),
		std::back_inserter(output));

	return output;
}

int main()
{
	using namespace std::string_literals;
	std::vector<std::string> expected{ "this", "is", "a", "sample" };

	assert(expected == Split("this is a sample"s, R"~(\s+)~"));
	assert(expected == Split("this,is a.sample!!"s, R"~([,.! ]+)~"));

	return 0;
}