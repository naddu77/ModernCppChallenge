#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>

auto Eratosthenes(int const limit)
{
	std::vector is_prime(limit + 1, true);

	if (std::size(is_prime) > 2u)
	{
		is_prime[0] = false;
		is_prime[1] = false;

		auto sqrtn = static_cast<int>(std::sqrt(limit));

		for (auto i = 2; i <= sqrtn; ++i)
			if (is_prime[i])
				for (auto j = i * i; j <= limit; j += i)
					is_prime[j] = false;
	}
	else
		std::fill(std::begin(is_prime), std::end(is_prime), false);

	return is_prime;
}

auto MakePrimeNumbers(std::vector<bool> const& is_prime)
{
	std::vector<int> prime_numbers;

	for (auto i = 2u; i < std::size(is_prime); ++i)
		if (is_prime[i])
			prime_numbers.push_back(i);

	return prime_numbers;
}

int main()
{
	auto limit = 0;

	std::cout << "Upper limit: ";
	std::cin >> limit;

	auto const is_prime = Eratosthenes(limit);

	for (auto i = 2; i < static_cast<int>(std::size(is_prime)) - 6; ++i)
		if (is_prime[i] && is_prime[i + 6])
			std::cout << i << ", " << i + 6 << std::endl;

	return 0;
}