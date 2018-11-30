// 055_Zip_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// - Note
// Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
	return os << '{' << p.first << ',' << p.second << '}';
}

int main()
{
	std::vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> v2{ 1, 1, 3, 5, 8, 13, 21 };

	ranges::copy(ranges::view::zip(v1, v2), ranges::ostream_iterator<std::pair<int, int>>(std::cout, "\n"));

	return 0;
}