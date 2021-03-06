#include "pch.h"
#include <iostream>
#include <set>
#include <numeric>

auto MakeProperDivisors(int const number)
{
	std::set<int> result{ 1 };

	for (auto i = 2; i <= std::sqrt(number); ++i)
		if (number % i == 0)
		{
			result.insert(i);
			result.insert(number / i);
		}

	return result;
}

inline auto Sum(std::set<int> const& s)
{
	return std::reduce(std::cbegin(s), std::cend(s));
}

void PrintAbundantNumbers(int const limit)
{
	for (auto number = 10; number <= limit; ++number)
	{
		auto const sum_of_proper_divisors = Sum(MakeProperDivisors(number));
		
		if (sum_of_proper_divisors > number)
			std::cout << number << ", abundance=" << sum_of_proper_divisors - number << std::endl;
	}
}

int main()
{
	auto limit = 0;

	std::cout << "Upper limit: ";
	std::cin >> limit;

	PrintAbundantNumbers(limit);

	return 0;
}