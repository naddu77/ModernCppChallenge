// 012_Largest_Collatz_sequence.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <vector>
#include <iostream>
#include <algorithm>

auto LongestCollatzSub(std::pair<unsigned long long, std::size_t> p, unsigned long i) -> std::pair<unsigned long long, std::size_t>
{
	return p.first != 1ull && p.first >= i ? LongestCollatzSub({ p.first % 2ull == 0 ? p.first / 2ull : p.first * 3ull + 1ull, p.second + 1 }, i) : p;
}

auto LongestCollatz(unsigned long const limit) -> std::pair<std::size_t, unsigned long long>
{
	if (limit < 2)
		throw std::invalid_argument("invalid_argumet");

	std::vector cache(limit + 1ul, 0ull);
	auto i = 1u;

	std::generate(std::begin(cache) + 2, std::end(cache), [&cache, &i]() {
		++i;

		auto const[n, steps] = LongestCollatzSub({ i, 0l }, i);

		return steps + cache[static_cast<std::size_t>(n)];
	});

	auto max = std::max_element(std::cbegin(cache), std::cend(cache));

	return { std::distance(std::cbegin(cache), max), *max };
}

int main()
{
	auto largest_collatz = LongestCollatz(1'000'000);

	std::cout << largest_collatz.first << ", " << largest_collatz.second << std::endl;

	return 0;
}