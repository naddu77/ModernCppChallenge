// 067_Validating_passwords.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용
// - Fold expression 이용

#include "pch.h"
#include <iostream>
#include <string_view>
#include <cctype>
#include <vector>
#include <functional>
#include <cassert>
#include <range/v3/algorithm.hpp>

using namespace std::string_view_literals;

template <typename... Validators>
bool IsValidPassword(std::string_view password, Validators&&... validators)
{
	return (... && validators(password));
}

int main()
{
	auto password = "abc123!@#"sv;

	auto digit_password_validator = [](auto str) {
		return ranges::any_of(str, std::isdigit);
	};
	
	auto length_validator = [](auto const length) {
		return [=](auto str) {
			return ranges::distance(str) >= length;
		};
	};

	auto upper_case_password_validator = [](auto str) {
		return ranges::any_of(str, std::isupper);
	};

	auto lower_case_password_validator = [](auto str) {
		return ranges::any_of(str, std::islower);
	};
	
	auto symbol_password_validator = [](auto str) {
		return ranges::any_of(str, std::ispunct);
	};

	assert(IsValidPassword("abc123!@#"sv, digit_password_validator, length_validator(8)));
	assert(!IsValidPassword("abcde!@#"sv, digit_password_validator, length_validator(8)));
	assert(IsValidPassword("Abc123!@#"sv,
		symbol_password_validator,
		upper_case_password_validator,
		lower_case_password_validator,
		digit_password_validator,
		length_validator(8)
		));
	assert(!IsValidPassword("Abc123567"sv,
		symbol_password_validator,
		upper_case_password_validator,
		lower_case_password_validator,
		digit_password_validator,
		length_validator(8)
	));

	return 0;
}