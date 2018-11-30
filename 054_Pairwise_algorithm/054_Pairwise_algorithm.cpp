// 054_Pairwise_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <iterator>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
	return os << '{' << p.first << ',' << p.second << '}';
}

int main()
{
	std::vector<int> v{ 1, 1, 3, 5, 8, 13, 21 };

	ranges::copy(ranges::view::chunk(v, 2)
		| ranges::view::filter([](auto e) { return ranges::distance(e) > 1; })
		| ranges::view::transform([](auto e) { return std::pair{ e[0], e[1] }; }),
		ranges::ostream_iterator<std::pair<int, int>>(std::cout, "\n"));

	return 0;
}