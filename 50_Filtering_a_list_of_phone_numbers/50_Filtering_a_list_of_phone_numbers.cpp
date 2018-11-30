// 50_Filtering_a_list_of_phone_numbers.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>

int main()
{
	std::vector<std::string> numbers{
		 "+40744909080",
		 "44 7520 112233",
		 "+44 7555 123456",
		 "40 7200 123456",
		 "7555 123456"
	};

	auto result = filter_numbers(numbers, "44");

	for (auto const & number : result)
	{
		std::cout << number << std::endl;
	}

	return 0;
}
