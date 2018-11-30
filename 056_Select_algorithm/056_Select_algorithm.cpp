// 056_Select_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <string>

struct Book
{
	int id;
	std::string title;
	std::string author;
};

template <typename F>
auto Select(F&& f)
{
	return ranges::make_pipeable([=](auto&& rng) {
		return std::forward<decltype(rng)>(rng) | ranges::view::transform(f);
	});
}

int main()
{
	std::vector<Book> books{
	  {101, "The C++ Programming Language", "Bjarne Stroustrup"},
	  {203, "Effective Modern C++", "Scott Meyers"},
	  {404, "The Modern C++ Programming Cookbook", "Marius Bancila"} };

	ranges::copy(
		books | Select([](auto const& b) { return b.title; }),
		ranges::ostream_iterator<std::string>(std::cout, "\n")
	);

	return 0;
}