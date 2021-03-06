// 008_Armstrong_numbers.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <array>
#include <numeric>
#include <vector>
#include <string>

auto MakeThreeDigits(void)
{
	std::vector<int> three_digits(900);

	std::iota(std::begin(three_digits), std::end(three_digits), 100);

	return three_digits;
}

auto MakeArmstrongNumbers(std::vector<int> const& three_digits)
{
	std::vector<int> armstrong_numbers;

	for (auto const number : three_digits)
	{
		auto temp = std::to_string(number);
		auto arm = std::reduce(std::cbegin(temp), std::cend(temp), 0, [](auto const acc, auto const e) { 
			auto n = e - '0';

			return acc + n * n * n;
		});

		if (number == arm)
			armstrong_numbers.push_back(arm);
	}

	return armstrong_numbers;
}

template <typename Cont>
void PrintVector(Cont const& l)
{
	for (auto const& e : l)
		std::cout << e << std::endl;
}

int main()
{
	PrintVector(MakeArmstrongNumbers(MakeThreeDigits()));

	return 0;
}
