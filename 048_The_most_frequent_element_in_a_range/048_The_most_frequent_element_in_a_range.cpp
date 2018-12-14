// 048_The_most_frequent_element_in_a_range.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
	os << '(' << p.first << "," << p.second << ')';

	return os;
}

auto Sorted()
{
	return ranges::make_pipeable([](auto&& rng) {
		return std::forward<decltype(rng)>(rng)
			| ranges::copy
			| ranges::action::sort;
	});
}

auto ValueBy()
{
	return ranges::view::group_by([](int const a, int const b) { return a == b; });
}

auto ToPair()
{
	return ranges::view::transform([](auto const rng) { return std::pair{ rng.front(), ranges::distance(rng) }; });
}

auto FilterMostFrequentNumber()
{
	return ranges::make_pipeable([=](auto &&rng) {
		auto most_frequent_number = ranges::max_element(rng, [](auto const a, auto const b) { return a.second < b.second; });
		
		return std::forward<decltype(rng)>(rng) | ranges::view::filter([m = (*most_frequent_number).second](auto const e) { return e.second == m; });
	});
}

int main()
{
	auto range = std::vector{ 1,1,3,5,8,13,3,5,8,8,5 };

	auto sorted = range
		| Sorted()
		;

	auto result = sorted
		| ValueBy()
		| ToPair()
		| FilterMostFrequentNumber()
		;

	std::cout << result << std::endl;

	return 0;
}