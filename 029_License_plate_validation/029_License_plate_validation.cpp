// 029_License_plate_validation.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: 함수의 재사용성을 고려

#include "pch.h"
#include <iostream>
#include <string>
#include <regex>
#include <cassert>

auto IsValidString(std::string const& str, std::string const& d)
{
	std::regex txt_regex(d);

	return std::regex_match(str, txt_regex);
}

auto IsValidLicensePlateNumber(std::string const& license_plate_number)
{
	return IsValidString(license_plate_number, R"~([A-Z]{3}-[A-Z]{2} \d{3,4})~");
}

auto ExtractString(std::string const& str, std::string const& d)
{
	std::vector<std::string> result;
	std::regex txt_regex(d);

	for (auto it = std::sregex_iterator(std::cbegin(str), std::cend(str), txt_regex);
		it != std::sregex_iterator{}; ++it)
		result.push_back(it->str());

	return result;
}

auto ExtractLicensePlateNumbers(std::string const& license_plate_number)
{
	return ExtractString(license_plate_number, R"~([A-Z]{3}-[A-Z]{2} \d{3,4})~");
}

int main()
{
	{
		assert(IsValidLicensePlateNumber("ABC-DE 123"));
		assert(IsValidLicensePlateNumber("ABC-DE 1234"));
		assert(!IsValidLicensePlateNumber("ABC-DE 12345"));
		assert(!IsValidLicensePlateNumber("abc-de 1234"));
	}
	
	{
		std::vector<std::string> expected{ "AAA-AA 123", "ABC-DE 1234", "XYZ-WW 0001" };
		std::string text("AAA-AA 123qwe-ty 1234 ABC-DE 123456..XYZ-WW 0001");

		assert(expected == ExtractLicensePlateNumbers(text));
	}
}