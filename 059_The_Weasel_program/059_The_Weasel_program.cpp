// 059_The_Weasel_program.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <array>
#include <algorithm>
#include <iomanip>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
	return os << '{' << p.first << ',' << p.second << '}';
}

void Run(std::string const& target, int const children_count, double const mutating_percent)
{
	std::vector<char> char_table = ranges::view::iota('A', 'Z' + 1);

	char_table |= ranges::action::push_back(' ');

	std::random_device rd;
	std::uniform_int_distribution chardist(0, 26);
	std::uniform_real_distribution ratedist(0.0, 100.0);
	auto step = 1;

	std::string parent = ranges::view::generate([char_table, chardist, &rd] { return char_table[chardist(rd)]; })
		| ranges::view::take(std::size(target));

	do
	{
		std::vector<std::string> children;

		ranges::generate_n(ranges::back_inserter(children), children_count, [parent, mutating_percent, char_table, chardist, ratedist, &rd] {
			return parent
				| ranges::view::transform([mutating_percent, char_table, chardist, ratedist, &rd](auto const c) { return ratedist(rd) > mutating_percent ? c : char_table[chardist(rd)]; });
		});

		parent = *ranges::max_element(children, [target](auto const& a, auto const& b) {
			auto GetScore = ranges::make_pipeable([](auto&& pairs) {
				return ranges::count_if(std::forward<decltype(pairs)>(pairs), [](auto const& p) { return p.first == p.second; });
			});

			return (ranges::view::zip(a, target) | GetScore) < (ranges::view::zip(b, target) | GetScore);
		});

		std::cout << std::setw(5) << std::setfill(' ') << step << ": " << parent << std::endl;

		++step;
	} while (parent != target);
}

int main()
{
	Run("METHINKS IT IS LIKE A WEASEL", 100, 5);

	return 0;
}