// 011_Converting_numerical_values_to_Roman.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>

std::string ToRoman(unsigned int value)
{
	static std::vector<std::pair<unsigned int, std::string_view>> roman{
		{ 1000u, "M" }, { 900u, "CM" }, { 500u, "D" },{ 400u, "CD" },
		{ 100u, "C" }, { 90u, "XC" }, { 50u, "L" }, { 40u, "XL" },
		{ 10u, "X" }, { 9u, "IX" }, { 5u, "V" }, { 4u, "IV" }, { 1u, "I" }
	};

	return std::reduce(std::cbegin(roman), std::cend(roman), std::string{}, [&value](auto&& acc, auto const& p) {
		while (value >= p.first)
		{
			acc += p.second;
			value -= p.first;
		}
		
		return std::move(acc);
	});
}

int main()
{
	for (int i = 1; i <= 100; ++i)
	{
		std::cout << i << "\t" << ToRoman(i) << std::endl;
	}

	auto number = 0;

	std::cout << "number:";
	std::cin >> number;
	std::cout << ToRoman(number) << std::endl;
}