// 010_Gray_code.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>

inline auto GrayEncode(unsigned int const num)
{
	return num ^ (num >> 1);
}

auto GrayDecode(unsigned int num)
{
	num ^= num >> 16;
	num ^= num >> 8;
	num ^= num >> 4;
	num ^= num >> 2;
	num ^= num >> 1;

	return num;
}

inline std::string ToBinary(unsigned int value, int const digits)
{
	return std::bitset<32>(value).to_string().substr(32 - digits, digits);
}

int main()
{
	std::cout << "Number\tBinary\tGray\tDecoded\n";
	std::cout << "------\t------\t----\t-------\n";

	for (auto n = 0u; n < 32; ++n)
	{
		auto encg = GrayEncode(n);
		auto decg = GrayDecode(encg);

		std::cout << std::setw(4) << n << "\t" << ToBinary(n, 5) << "\t" << ToBinary(encg, 5) << "\t" << std::setw(5) << decg << "\n";
	}
}