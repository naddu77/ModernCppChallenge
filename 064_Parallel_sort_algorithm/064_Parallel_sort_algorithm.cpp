// 064_Parallel_sort_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - parallel std::sort가 가장 빠름

#include "pch.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <array>
#include <future>
#include <sstream>
#include <iterator>
#include <execution>
#include <chrono>

using namespace std::chrono_literals;

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> const& p)
{
	os << '(' << *(p.first) << ',' << *(p.second) << ')';

	return os;
}

class FunctionExecutionTimeCalculator
{
public:
	template <typename Unit = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock, typename Func, typename... Ts>
	decltype(auto) operator()(Func&& func, Ts&&... args)
	{
		auto const start_time = Clock::now();

		if constexpr (std::is_same_v<std::invoke_result_t<Func, Ts...>, void>)
		{
			std::invoke(std::forward<Func>(func), std::forward<Ts>(args)...);

			function_execution_times = std::chrono::duration_cast<Unit>(Clock::now() - start_time).count();
		}
		else
		{
			decltype(auto) result = std::invoke(std::forward<Func>(func), std::forward<Ts>(args)...);

			function_execution_times = std::chrono::duration_cast<Unit>(Clock::now() - start_time).count();

			return result;
		}
	}

	inline long long GetFunctionExecutionTime(void) const
	{
		return function_execution_times;
	}

private:
	long long function_execution_times;
};

template <class RandomIt>
RandomIt partition(RandomIt first, RandomIt last)
{
	auto pivot = *first;
	auto i = first + 1;
	auto j = last - 1;
	while (i <= j)
	{
		while (i <= j && *i <= pivot) i++;
		while (i <= j && *j > pivot) j--;
		if (i < j) std::iter_swap(i, j);
	}

	std::iter_swap(i - 1, first);

	return i - 1;
}

template <class RandomIt>
void pquicksort(RandomIt first, RandomIt last)
{
	if (first < last)
	{
		auto p = partition(first, last);

		if (last - first <= 100000)
		{
			pquicksort(first, p);
			pquicksort(p + 1, last);
		}
		else
		{
			auto f1 = std::async(std::launch::async,
				[first, p]() { pquicksort(first, p); });
			auto f2 = std::async(std::launch::async,
				[last, p]() { pquicksort(p + 1, last); });
			f1.wait();
			f2.wait();
		}
	}
}

int main()
{
	std::random_device rd;
	std::mt19937 mt;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data),
		std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	mt.seed(seq);
	std::uniform_int_distribution<> ud(1, 1000);

	const size_t count = 100000;
	std::vector<int> data(count);
	std::generate_n(std::begin(data), count, [&mt, &ud]() {return ud(mt); });

	std::vector<int> data2(count);
	std::vector<int> data3(count);

	std::copy(std::cbegin(data), std::cend(data), std::begin(data2));
	std::copy(std::cbegin(data), std::cend(data), std::begin(data3));

	std::vector<FunctionExecutionTimeCalculator> function_execution_times(3);
	std::vector<std::pair<std::string, long long>> result;

	function_execution_times[0](pquicksort<decltype(std::begin(data))>, std::begin(data), std::end(data));
	function_execution_times[1](std::sort<decltype(std::begin(data2))>, std::begin(data2), std::end(data2));
	function_execution_times[2](
		[](auto&& p, auto&& b, auto&& e) { return std::sort(std::forward<decltype(p)>(p), std::forward<decltype(b)>(b), std::forward<decltype(e)>(e)); },
		std::execution::par, std::begin(data3), std::end(data3));

	result.emplace_back(
		"pquicksort",
		function_execution_times[0].GetFunctionExecutionTime()
	);
	result.emplace_back(
		"std::sort",
		function_execution_times[1].GetFunctionExecutionTime()
	);

	result.emplace_back(
		"std::sort(par)",
		function_execution_times[2].GetFunctionExecutionTime()
	);

	std::vector<std::string> strs;

	std::transform(
		std::cbegin(result),
		std::cend(result),
		std::back_inserter(strs),
		[](auto const& e) {
			std::stringstream ss;

			ss << e.first << ": " << e.second << " ms.";

			return ss.str();
	});

	std::copy(
		std::cbegin(strs),
		std::cend(strs),
		std::ostream_iterator<std::string>(std::cout, "\n")
	);

	return 0;
}