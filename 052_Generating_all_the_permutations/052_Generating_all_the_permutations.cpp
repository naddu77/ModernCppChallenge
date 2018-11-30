// 052_Generating_all_the_permutations.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - STL 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <string>


int main()
{
	using namespace std::string_literals;

	auto input = "main"s;

	do
	{
		std::cout << input << std::endl;
	} while (ranges::next_permutation(input));

	return 0;
}
