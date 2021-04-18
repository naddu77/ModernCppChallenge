// 089_Vigenère_cipher.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <range/v3/all.hpp>

using namespace std::string_literals;

std::string build_vigenere_table()
{
	std::string alphabet{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

	std::vector<int> ints = ranges::view::ints(0)
		| ranges::view::take(26)
		;

	std::vector<std::string> alphabets = ranges::view::repeat_n(alphabet, 26);

	std::vector<std::string> table = ranges::view::zip_with([](auto&& i, auto&& alphabet) {
		ranges::rotate(alphabet, ranges::begin(alphabet) + i);

		return alphabet;
	}, ints, alphabets)
	;

	return table | ranges::view::join;
}

std::string vigenere_encrypt(std::string_view text, std::string_view key)
{
	std::string result;
	result.reserve(text.length());
	static auto table = build_vigenere_table();

	for (size_t i = 0; i < text.length(); ++i)
	{
		auto row = key[i%key.length()] - 'A';
		auto col = text[i] - 'A';

		result += table[row * 26 + col];
	}

	return result;
}

std::string vigenere_decrypt(std::string_view text, std::string_view key)
{
	std::string result;
	result.reserve(text.length());
	static auto table = build_vigenere_table();

	for (size_t i = 0; i < text.length(); ++i)
	{
		auto row = key[i%key.length()] - 'A';

		for (size_t col = 0; col < 26; col++)
		{
			if (table[row * 26 + col] == text[i])
			{
				result += 'A' + static_cast<char>(col);
				break;
			}
		}
	}

	return result;
}

int main()
{
	auto text = "THECPPCHALLENGER";
	auto enc = vigenere_encrypt(text, "SAMPLE");
	auto dec = vigenere_decrypt(enc, "SAMPLE");
	assert(text == dec);

	return 0;
}
