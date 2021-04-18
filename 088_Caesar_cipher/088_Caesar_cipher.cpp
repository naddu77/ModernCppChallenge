// 088_Caesar_cipher.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <cassert>
#include <cctype>
#include <range/v3/all.hpp>

using namespace std::string_literals;

int main()
{
	auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"s;

	auto caesar_encrypt = [](int const i) {
		return ranges::view::transform([i](auto const c) -> char {
			return 'A' + (c - 'A' + i) % 26;
		});
	};

	auto caesar_decrypt = [](int const i) {
		return ranges::view::transform([i](auto const c) -> char {
			return 'A' + (26 + c - 'A' - 3) % 26;
		});
	};

	auto is_alpha_and_upper = ranges::view::filter([](auto const c) {
		return std::isalpha(c) && std::isupper(c);
	});

	std::string encoded = text
		| is_alpha_and_upper
		| caesar_encrypt(3)
		;

	std::string decoded = encoded
		| is_alpha_and_upper
		| caesar_decrypt(3)
		;

	for (int i = 1; i <= 26; ++i)
		assert(text == decoded);

	return 0;
}