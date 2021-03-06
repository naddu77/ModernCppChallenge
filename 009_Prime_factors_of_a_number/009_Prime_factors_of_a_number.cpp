// 009_Prime_factors_of_a_number.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <numeric>
#include <vector>

auto Eratosthenes(int const limit)
{
	std::vector min_factors(limit + 1, -1);

	if (std::size(min_factors) > 2u)
	{
		min_factors[0] = -1;
		min_factors[1] = -1;
		std::iota(std::begin(min_factors) + 2, std::end(min_factors), 2);
			   
		auto sqrtn = static_cast<int>(std::sqrt(limit));

		for (auto i = 2; i <= sqrtn; ++i)
			if (min_factors[i] == i)
				for (auto j = i * i; j <= limit; j += i)
					if(min_factors[j] == j)
						min_factors[j] = i;
	}
	else
		return std::vector(2, -1);

	return min_factors;
}

auto GetFactors(int n)
{
	std::vector<int> factors;

	auto const min_factors = Eratosthenes(n);

	while (n > 1)
	{
		factors.push_back(min_factors[n]);
		n /= min_factors[n];
	}

	return factors;
}

template <typename Cont>
void PrintVector(Cont const& l)
{
	for (auto const& e : l)
		std::cout << e << std::endl;
}

int main()
{
	auto limit = 0;

	std::cout << "Number: ";
	std::cin >> limit;

	PrintVector(GetFactors(limit));

	return 0;
}