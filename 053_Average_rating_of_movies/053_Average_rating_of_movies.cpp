// 053_Average_rating_of_movies.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

struct Moive
{
	int              id;
	std::string      title;
	std::vector<int> ratings;
};

int main()
{
	std::vector<Moive> movies
	{
	   { 101, "The Matrix", {10, 9, 10, 9, 9, 8, 7, 10, 5, 9, 9, 8} },
	   { 102, "Gladiator", {10, 5, 7, 8, 9, 8, 9, 10, 10, 5, 9, 8, 10} },
	   { 103, "Interstellar", {10, 10, 10, 9, 3, 8, 8, 9, 6, 4, 7, 10} }
	};

	auto sort_ratings = [] {
		return ranges::view::transform([](auto e) {
			ranges::sort(e.ratings);

			return std::move(e);
		});
	};

	auto filter_highest_and_lowest_ratings = [](auto const percent) {
		return ranges::view::transform([percent](auto e) {
			auto ratings_count = ranges::distance(e.ratings);
			auto remove_count = static_cast<std::size_t>(ratings_count * (percent / 100.0) + 0.5);

			e.ratings |= ranges::action::take(ratings_count - remove_count) | ranges::action::drop(remove_count);

			return std::move(e);
		});
	};

	auto to_histogram = [] {
		return ranges::view::transform([](auto e) {
			return std::tuple{ e.id, e.title, ranges::accumulate(e.ratings, 0.0) / ranges::distance(e.ratings) };
		});
	};

	auto to_print = [] {
		return ranges::view::transform([](auto e) {
			std::stringstream ss;

			ss << std::get<1>(e) << " : "
				<< std::fixed << std::setprecision(1)
				<< std::get<2>(e);

			return ss.str();
		});
	};

	ranges::copy(movies
		| sort_ratings()
		| filter_highest_and_lowest_ratings(5)
		| to_histogram()
		| to_print(),
		ranges::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}
