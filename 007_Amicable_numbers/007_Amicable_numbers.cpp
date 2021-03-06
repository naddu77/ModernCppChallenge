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

void PrintAmicableNumbers(int const limit)
{
	for (auto number = 4; number < limit; ++number)
	{
		auto const sum_of_proper_divisors = Sum(MakeProperDivisors(number));

		if (sum_of_proper_divisors < limit)
		{
			auto const sum_of_proper_divisors2 = Sum(MakeProperDivisors(sum_of_proper_divisors));

			if (sum_of_proper_divisors2 == number && number != sum_of_proper_divisors)
				std::cout << number << ", " << sum_of_proper_divisors << std::endl;
		}
	}
}

int main()
{
	PrintAmicableNumbers(1'000'000);

	return 0;
}