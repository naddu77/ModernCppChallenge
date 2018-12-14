// 061_Parallel_transform_algorithm.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Parallel std::transform이 가장 빠름

#include "pch.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <functional>
#include <execution>

using namespace std::chrono_literals;

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

template <typename RandomAccessIterator, typename F>
void ptransform(RandomAccessIterator begin, RandomAccessIterator end,
	F&& f)
{
	auto size = std::distance(begin, end);
	if (size <= 10000)
	{
		std::transform(begin, end, begin, std::forward<F>(f));
	}
	else
	{
		std::vector<std::thread> threads;
		int thread_count = std::thread::hardware_concurrency();
		auto first = begin;
		auto last = first;
		size /= thread_count;
		for (int i = 0; i < thread_count; ++i)
		{
			first = last;
			if (i == thread_count - 1) last = end;
			else std::advance(last, size);

			threads.emplace_back([first, last, &f]() {
				std::transform(first, last, first, std::forward<F>(f));
			});
		}

		for (auto & t : threads) t.join();
	}
}

template <typename T, typename F>
std::vector<T> palter(std::vector<T> data, F&& f)
{
	ptransform(std::begin(data), std::end(data), std::forward<F>(f));

	return data;
}

template <typename T, typename F>
std::vector<T> OriginalTransform(std::vector<T> data, F&& f)
{
	std::transform(std::begin(data), std::end(data), std::begin(data), std::forward<F>(f));

	return data;
}

template <typename T, typename F>
std::vector<T> ParallelTransform(std::vector<T> data, F&& f)
{
	std::transform(std::execution::par, std::begin(data), std::end(data), std::begin(data), std::forward<F>(f));

	return data;
}

int main()
{
	std::vector<int> data(1000000);

	std::iota(std::begin(data), std::end(data), 1);

	auto pred = [](int const e) -> int {return e * e; };
	std::vector<FunctionExecutionTimeCalculator> function_execution_time_calculator(3);

	auto result1 = function_execution_time_calculator[0](palter<int, int(*)(int const)>, data, pred);
	auto result2 = function_execution_time_calculator[1](OriginalTransform<int, int(*)(int const)>, data, pred);
	auto result3 = function_execution_time_calculator[2](ParallelTransform<int, int(*)(int const)>, data, pred);

	for(auto const& function_execution_time : function_execution_time_calculator)
		std::cout << function_execution_time.GetFunctionExecutionTime() << " ms." << std::endl;

	return 0;
}