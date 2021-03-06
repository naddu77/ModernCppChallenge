// 013_Computing_the_value_of_Pi.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <vector>

auto ComputeTheValueOfPi(int const samples = 1'000'000)
{
	std::random_device rd;
	std::uniform_real_distribution<> dist;
	auto GetRandomNumber = [&rd, dist] { return dist(rd); };
	std::vector<std::pair<double, double>> data(samples);
	std::vector<double> result;

	std::generate(std::begin(data), std::end(data), [GetRandomNumber] { return std::pair{ GetRandomNumber(), GetRandomNumber() }; });
	std::transform(std::cbegin(data), std::cend(data), std::back_inserter(result), [](auto const p) { return p.first * p.first + p.second * p.second; });

	return 4.0 * std::count_if(std::cbegin(result), std::cend(result), [](auto const v) { return v <= 1; }) / std::size(result);
}

int main()
{
	std::cout << ComputeTheValueOfPi() << std::endl;

	return 0;
}
