// 049_Text_histogram.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
	os << '(' << p.first << "," << p.second << ')';

	return os;
}

int main()
{
	using namespace std::string_literals;

	auto input = R"(Lorem ipsum dolor sit amet, consectetur
					adipiscing elit, sed do eiusmod tempor incididunt ut labore et
					dolore magna aliqua.)"s;

	auto const refined = input
		| ranges::move
		| ranges::action::transform([](auto const c) -> char { return std::tolower(c); })
		| ranges::action::remove_if([](auto const c) { return !std::isalpha(c); })
		| ranges::action::sort
		;

	auto const result = refined
		| ranges::view::group_by([](auto const a, auto const b) { return a == b; })
		| ranges::view::transform([](auto const rng) { return std::pair{ rng.front(), ranges::distance(rng) }; })
		| ranges::view::transform([total = ranges::distance(refined)](auto const p) { return std::pair{ p.first, static_cast<double>(p.second) / total }; })
		;

	std::cout << result << std::endl;
	std::cout	// total 빈도가 1인지 확인
		<< ranges::accumulate(result, double{}, [](auto const acc, auto const e) { return acc + e.second; })
		<< std::endl;

	return 0;
}