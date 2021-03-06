#include "pch.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

int main()
{
	auto limit = 0u;

	std::cin >> limit;

	std::vector<decltype(limit)> numbers(limit);

	auto Pred = [](auto const n) {
		return [n](auto const i) {
			return i % n == 0;
		};
	};

	std::iota(std::begin(numbers), std::end(numbers), 1);
	std::cout << "sum=" << std::reduce(std::begin(numbers), std::partition(std::partition(std::begin(numbers), std::end(numbers), Pred(3)), std::end(numbers), Pred(5))) << std::endl;

	return 0;
}