// 057_Sort_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>

int main()
{
	std::vector<int> v{ 1,5,3,8,6,2,9,7,4 };

	v |= ranges::action::sort;

	std::cout << ranges::view::all(v) << std::endl;

	return 0;
}
