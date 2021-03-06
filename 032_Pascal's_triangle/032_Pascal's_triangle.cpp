// 032_Pascal's_triangle.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Zip을 이용하여 Pascal's Triangle을 만듦
//   [1, 1]
//      [1, 1]
//   [1, 2, 1]
//      [1, 2, 1]
//   [1, 3, 3, 1]
//      [1, 3, 3, 1]
//   [1, 4, 6, 4, 1]

#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <sstream>

auto MakePascalTriangle(int const n)
{
	std::vector<std::vector<int>> pascal_triangle(n, std::vector<int>(1, 1));

	if (n > 1)
	{
		pascal_triangle[1].push_back(1);

		std::transform(std::next(std::cbegin(pascal_triangle)), std::prev(std::cend(pascal_triangle)),
			std::next(std::next(std::begin(pascal_triangle))),
			[](auto const& e) {
				std::vector<int> result(1, 1);

				std::transform(std::next(std::cbegin(e)), std::cend(e),
					std::cbegin(e),
					std::back_inserter(result),
					[](auto const a, auto const b) {
						return a + b;
					}
				);
				
				result.push_back(1);

				return result;
			}
		);
	}

	return pascal_triangle;
}

auto PascalTriangleToStringVector(std::vector<std::vector<int>> pascal_triangle)
{
	std::vector<std::string> str_pascal_triangle;

	std::transform(std::cbegin(pascal_triangle), std::cend(pascal_triangle), std::back_inserter(str_pascal_triangle),
		[](auto const& e) {
			std::stringstream ss;

			std::copy(std::cbegin(e), std::cend(e), std::ostream_iterator<int>(ss, "    "));

			auto result = ss.str();

			if (!std::empty(result))
			{
				result.pop_back();
				result.pop_back();
				result.pop_back();
				result.pop_back();
			}

			return result;
		}
	);

	return str_pascal_triangle;
}

void PrintPascalTriangle(int const n)
{
	auto str_pascal_triangle = PascalTriangleToStringVector(MakePascalTriangle(n));
	auto const length = std::size(str_pascal_triangle.back());
	
	std::transform(std::begin(str_pascal_triangle), std::end(str_pascal_triangle),
		std::begin(str_pascal_triangle),
		[length](auto const& e) {
			std::stringstream ss;

			std::fill_n(std::ostream_iterator<char>(ss), (length - std::size(e)) >> 1, ' ');
			ss << e;

			return ss.str();
		}
	);

	std::copy(std::cbegin(str_pascal_triangle), std::cend(str_pascal_triangle), std::ostream_iterator<std::string>(std::cout, "\n"));
}

unsigned int number_of_digits(unsigned int const i)
{
	return i > 0 ? (int)log10((double)i) + 1 : 1;
}

void print_pascal_triangle(int const n)
{
	for (int i = 0; i < n; i++)
	{
		auto x = 1;
		std::cout << std::string((n - i - 1)*(n / 2), ' ');
		for (int j = 0; j <= i; j++)
		{
			auto y = x;
			x = x * (i - j) / (j + 1);
			auto maxlen = number_of_digits(x) - 1;
			std::cout << y << std::string(n - 1 - maxlen - n % 2, ' ');
		}
		std::cout << std::endl;
	}
}

int main()
{
	int n = 0;

	std::cout << "Levels (up to 10): ";
	std::cin >> n;

	if (n > 10)
		std::cout << "Value too large" << std::endl;
	else
	{
		PrintPascalTriangle(n);
		print_pascal_triangle(n);
	}

	return 0;
}