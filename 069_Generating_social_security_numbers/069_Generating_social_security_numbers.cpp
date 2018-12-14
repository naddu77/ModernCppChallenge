// 069_Generating_social_security_numbers.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용
// Question
// - Lambda Expression을 이용하여 만든 Lambda Expression내의 static variable은 왜 공유될까?
// - 저자 코드 오류가 있는 듯

#include "pch.h"
#include <iostream>
#include <string_view>
#include <string>
#include <random>
#include <map>
#include <algorithm>
#include <cmath>
#include <range/v3/all.hpp>

using namespace std::string_literals;

enum class SexType
{
	Male,
	Female,
	//ThirdSex
};

//class RandomDigitsGenerator
//{
//public:
//	RandomDigitsGenerator(int const number_of_digits, std::random_device& rd)
//		: ud(static_cast<int>(std::pow(10, number_of_digits - 1)),
//			static_cast<int>(std::pow(10, number_of_digits)) - 1
//		), rd(rd)
//	{
//
//	}
//
//	RandomDigitsGenerator(RandomDigitsGenerator const&) = default;
//
//	~RandomDigitsGenerator() = default;
//
//	std::string operator()(std::string const& str) {
//		if (auto iter = table.find(str); iter != std::end(table))
//			return std::to_string(table[str]);
//		else
//		{
//			table[str] = ud(rd);
//
//			return std::to_string(table[str]);
//		}
//	};
//
//private:
//	std::map<std::string, int> table;
//	std::uniform_int_distribution<int> ud;
//	std::random_device& rd;
//};

int main()
{
	std::random_device rd;

	//auto make_sex_digit_generator = [](auto const&... p) {
	//	return [=](SexType const sex_type) {
	//		std::map<SexType, std::string> table{
	//			std::make_pair(p.first, std::to_string(p.second))...
	//		};

	//		return table[sex_type];
	//	};
	//};

	//auto random_digits_generator = [&rd](int number_of_digits) {
	//	return [number_of_digits, &rd](std::string const& birthday) {
	//		static std::map<std::string, int> table;
	//		std::uniform_int_distribution<int> ud(
	//			static_cast<int>(std::pow(10, number_of_digits - 1)), 
	//			static_cast<int>(std::pow(10, number_of_digits)) - 1
	//		);

	//		if (auto iter = table.find(birthday); iter != std::end(table))
	//			return std::to_string(table[birthday]);
	//		else
	//		{
	//			table[birthday] = ud(rd);

	//			return std::to_string(table[birthday]);
	//		}
	//	};
	//};

	auto generate_sex_digit_for_n = [](SexType const sex_type) {
		std::map<SexType, std::string> table{
			{ SexType::Male, "7" },
			{ SexType::Female, "9" }
		};

		return table[sex_type];
	};

	auto generate_sex_digit_for_s = [](SexType const sex_type) {
		std::map<SexType, std::string> table{
			{ SexType::Male, "2" },
			{ SexType::Female, "1" }
		};

		return table[sex_type];
	};

	auto generate_five_random_digits = [&rd](std::string const& birthday) {
		static std::map<std::string, int> table;
		std::uniform_int_distribution ud(10000, 99999);

		if (auto iter = table.find(birthday); iter != std::end(table))
			return std::to_string(table[birthday]);
		else
		{
			table[birthday] = ud(rd);

			return std::to_string(table[birthday]);
		}
	};

	auto generate_four_random_digits = [&rd](std::string const& birthday) {
		static std::map<std::string, int> table;
		std::uniform_int_distribution ud(1000, 9999);

		if (auto iter = table.find(birthday); iter != std::end(table))
			return std::to_string(table[birthday]);
		else
		{
			table[birthday] = ud(rd);

			return std::to_string(table[birthday]);
		}
	};

	auto make_social_number_generator = [](auto&& sex_digit_generator, auto&& random_digit_generator, auto const modular_value) {
		return [=](SexType const sex_type, std::string birthday) {
			auto generate_modular_value = [](int modular_value, auto rng) {
				std::vector<int> number_list = ranges::view::ints(2)
					| ranges::view::take(ranges::distance(rng));

				ranges::reverse(number_list);

				return std::to_string((modular_value - (ranges::accumulate(ranges::view::zip_with([](auto l1, auto l2) {
					return (l1 - '0') * l2;
				}, rng, number_list), 0) % modular_value)) % 10);
			};

			auto n1 = sex_digit_generator(sex_type);
			auto n2 = random_digit_generator(birthday);
			auto n1_2 = ranges::view::concat(n1, birthday, n2);
			auto n3 = generate_modular_value(modular_value, n1_2);

			return std::string{ ranges::view::concat(n1_2, n3) };
		};
	};

	auto generate_social_number_for_n = make_social_number_generator(
		//make_sex_digit_generator(std::pair{ SexType::Male, 7 }, std::pair{ SexType::Female, 9 }),
		//RandomDigitsGenerator(5, rd),
		//random_digits_generator(5),
		generate_sex_digit_for_n,
		generate_five_random_digits,
		11
	);

	auto generate_social_number_for_s = make_social_number_generator(
		//make_sex_digit_generator(std::pair{ SexType::Male, 2 }, std::pair{ SexType::Female, 1 }),
		//RandomDigitsGenerator(4, rd),
		generate_sex_digit_for_s,
		generate_four_random_digits,
		10
	);

	std::cout << generate_social_number_for_n(SexType::Male, "19850118") << std::endl;
	std::cout << generate_social_number_for_n(SexType::Male, "19850117") << std::endl;
	std::cout << generate_social_number_for_n(SexType::Male, "19850118") << std::endl;
	std::cout << generate_social_number_for_s(SexType::Male, "19850118") << std::endl;
	std::cout << generate_social_number_for_s(SexType::Male, "19850117") << std::endl;
	std::cout << generate_social_number_for_s(SexType::Male, "19850118") << std::endl;

	//auto generate_social_number_for_n = [=](SexType const sex_type, std::string birthday) {
	//	auto n1 = generate_sex_digit(sex_type);
	//	auto n2 = generate_five_random_digits(birthday);
	//	auto n1_2 = ranges::view::concat(n1, birthday, n2);
	//	auto n3 = generate_modular_value(n1_2);

	//	return std::string{ ranges::view::concat(n1_2, n3) };
	//};

	//std::cout << generate_social_number_for_n(SexType::Male, "19850118") << std::endl;

	//auto n1 = generate_sex_digit(SexType::Male);
	//auto birthday = "19850118"s;
	//auto n2 = generate_five_random_digits(birthday);
	//auto n1_2 = ranges::view::concat(n1, birthday, n2);
	//auto n3 = generate_modular_value(n1_2);

	//ranges::copy(ranges::view::concat(n1_2, n3), ranges::ostream_iterator<char>(std::cout));
	//std::cout << std::endl;
	

	return 0;
}