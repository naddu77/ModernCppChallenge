// 068_Generating_random_passwords.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용
// - Fold Expression
// - 저자 코드로는 최소 길이를 설정할 방법이 없는 것 같음.

#include "pch.h"
#include <iostream>
#include <string>
#include <random>
#include <range/v3/all.hpp>

template <typename... PasswordGenerators>
std::string GeneratePassword(int const minimum_length, PasswordGenerators&&... password_generators)
{
	auto password = (std::string{} + ... + std::invoke(password_generators));

	if (int diff = minimum_length - static_cast<int>(ranges::distance(password)); diff > 0)
	{
		std::random_device rd;
		std::uniform_int_distribution<int> ud(0, 127);

		password += ranges::view::generate([&rd, ud] { return ud(rd); })
			| ranges::view::filter(isprint)
			| ranges::view::take(diff);
	}

	ranges::shuffle(password);

	return password;
}

int main()
{
	std::random_device rd;

	auto random_pick = [&rd](std::string_view candidates) {
		std::uniform_int_distribution<std::size_t> ud(0, ranges::distance(candidates) - 1);

		return candidates[ud(rd)];
	};

	auto digit_generator = [random_pick] {
		return random_pick("0123456789");
	};

	auto symbol_generator = [random_pick] {
		return random_pick("!@#$%^&*(){}[]?<>");
	};

	auto upper_letter_generator = [random_pick] {
		return random_pick("ABCDEFGHIJKLMNOPQRSTUVXYWZ");
	};

	auto lower_letter_generator = [random_pick] {
		return random_pick("abcdefghijklmnopqrstuvxywz");
	};

	auto repeat_generator = [](auto const n, auto&& generator) {
		return [=] {
			std::string result;

			ranges::generate_n(ranges::back_inserter(result), n, generator);

			return result;
		};
	};

	std::cout << GeneratePassword(16,
		repeat_generator(2, symbol_generator),
		repeat_generator(2, digit_generator),
		repeat_generator(2, upper_letter_generator),
		repeat_generator(4, lower_letter_generator)
	) << std::endl;

	return 0;
}