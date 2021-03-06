// 024_String_to_binary_conversion.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: table과 std::transform이용

#include "pch.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <optional>
#include <string_view>
#include <algorithm>
#include <map>

std::vector<unsigned char> HexStringToContainer(std::string_view const hex_string)
{
	static std::map<char, unsigned char> hex_table{
		{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15},
		{'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
	};

	auto const length = std::size(hex_string);

	if (length % 2 != 0 || length == 0u)
		return {};

	std::vector<std::string_view> tokens;

	for (auto i = 0u; i < length; i += 2)
		tokens.push_back(hex_string.substr(i, 2));

	std::vector<unsigned char> result;

	std::transform(std::cbegin(tokens), std::cend(tokens), std::back_inserter(result), [](auto const e) {
		return (hex_table[e[0]] << 4) + hex_table[e[1]];
	});

	return result;
}

int main()
{
	std::vector<unsigned char> expected{ 0xBA, 0xAD, 0xF0, 0x0D, 0x42 };

	assert(HexStringToContainer("BAADF00D42") == expected);
	assert(HexStringToContainer("BaaDf00d42") == expected);

	return 0;
}
