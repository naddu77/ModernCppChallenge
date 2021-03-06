// 023_Binary_to_string_conversion.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: std::reduce 이용

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <numeric>
#include <sstream>
#include <iomanip>

template <typename Cont>
auto ToHexString(Cont const& l, bool const is_uppercase = false)
{
	return std::reduce(std::cbegin(l), std::cend(l), std::string{}, [is_uppercase](auto const acc, auto const& e) {
		std::stringstream ss;

		ss << std::hex
			<< (is_uppercase ? std::uppercase : std::nouppercase)
			<< std::setfill('0') 
			<< std::setw(2) 
			<< static_cast<int>(e);

		return std::move(acc + ss.str());
	});
}

int main()
{
	std::vector<unsigned char> v{ 0xBA, 0xAD, 0xF0, 0x0D };
	std::array<unsigned char, 6> a{ {1,2,3,4,5,6} };
	unsigned char buf[5] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

	assert(ToHexString(v, true) == "BAADF00D");
	assert(ToHexString(a, true) == "010203040506");
	assert(ToHexString(buf, true) == "1122334455");

	assert(ToHexString(v) == "baadf00d");
	assert(ToHexString(a) == "010203040506");
	assert(ToHexString(buf) == "1122334455");

	return 0;
}
