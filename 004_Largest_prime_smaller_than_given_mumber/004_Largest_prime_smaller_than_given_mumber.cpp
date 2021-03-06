#include "pch.h"
#include <iostream>
#include <numeric>
#include <vector>

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

inline auto GetPrimeNumber(std::vector<bool> const& is_prime)
{
	return std::find(std::crbegin(is_prime), std::crend(is_prime), true).base() - std::begin(is_prime) - 1;
}

int main()
{
	auto limit = 0;

	std::cout << "Upper limit: ";
	std::cin >> limit;
	   
	std::cout << "Largest prime:" << GetPrimeNumber(Eratosthenes(limit)) << std::endl;

	return 0;
}