// 062_Parallel_min_and_max_element.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 저자 코드가 가장 빠름

#include "pch.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <iterator>
#include <execution>
#include <tuple>
#include <random>
#include <variant>

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

template <typename Iterator, typename F>
auto pprocess(Iterator begin, Iterator end, F&& f)
{
	auto size = std::distance(begin, end);
	if (size <= 10000)
	{
		return std::forward<F>(f)(begin, end);
	}
	else
	{
		int thread_count = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		std::vector<typename std::
			iterator_traits<Iterator>::value_type>
			mins(thread_count);

		auto first = begin;
		auto last = first;
		size /= thread_count;
		for (int i = 0; i < thread_count; ++i)
		{
			first = last;
			if (i == thread_count - 1) last = end;
			else std::advance(last, size);

			threads.emplace_back([first, last, &f, &r = mins[i]]() {
				r = std::forward<F>(f)(first, last);
			});
		}

		for (auto & t : threads) t.join();

		return std::forward<F>(f)(std::begin(mins), std::end(mins));
	}
}

template <typename Iterator>
auto pmin(Iterator begin, Iterator end)
{
	return pprocess(begin, end,
		[](auto b, auto e) {return *std::min_element(b, e); });
}

template <typename Iterator>
auto pmax(Iterator begin, Iterator end)
{
	return pprocess(begin, end,
		[](auto b, auto e) {return *std::max_element(b, e); });
}

template <typename Iterator>
auto OriginalMin(Iterator begin, Iterator end)
{
	return *std::min_element(begin, end);
}

template <typename Iterator>
auto OriginalMax(Iterator begin, Iterator end)
{
	return *std::max_element(begin, end);
}

template <typename Iterator>
auto ParallelMin(Iterator begin, Iterator end)
{
	return *std::min_element(std::execution::par, begin, end);
}

template <typename Iterator>
auto ParallelMax(Iterator begin, Iterator end)
{
	return *std::max_element(std::execution::par, begin, end);
}

template <typename Iterator>
auto OriginalMinMax(Iterator begin, Iterator end)
{
	return std::minmax_element(begin, end);
}

template <typename Iterator>
auto ParallelMinMax(Iterator begin, Iterator end)
{
	return std::minmax_element(std::execution::par, begin, end);
}

template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

int main()
{
	std::random_device rd;
	std::vector<int> data(10'000'000);
	
	std::iota(std::begin(data), std::end(data), 1);

	std::shuffle(std::begin(data), std::end(data), rd);

	std::vector<FunctionExecutionTimeCalculator> function_execution_times(8);
	using MinMaxPair = std::pair<decltype(std::begin(data)), decltype(std::begin(data))>;
	std::vector<std::tuple<std::string, std::variant<int, MinMaxPair>>> result;
	std::size_t i = 0u;

	result.emplace_back("pmin", function_execution_times[i++](pmin<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Original Min", function_execution_times[i++](OriginalMin<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Parallel Min", function_execution_times[i++](ParallelMin<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("pmax", function_execution_times[i++](pmax<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Original Max", function_execution_times[i++](OriginalMax<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Parallel Max", function_execution_times[i++](ParallelMax<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Original Min and Max", function_execution_times[i++](OriginalMinMax<decltype(std::begin(data))>, std::begin(data), std::end(data)));
	result.emplace_back("Parallel Min and Max", function_execution_times[i++](ParallelMinMax<decltype(std::begin(data))>, std::begin(data), std::end(data)));

	std::vector<std::string> strs;

	std::transform(std::cbegin(result),
		std::cend(result), 
		std::cbegin(function_execution_times),
		std::back_inserter(strs), 
		[](std::tuple<std::string, std::variant<int, MinMaxPair>> const& e1, auto const& e2) {
			std::stringstream ss;

			ss << std::get<0>(e1) << ": ";
			ss << std::visit(Overloaded{
				[](int const e) { return std::to_string(e); },
				[](MinMaxPair const& p) { 
					std::stringstream ss;

					ss << p;

					return ss.str();
				}},
				std::get<1>(e1));
			ss << " (" << e2.GetFunctionExecutionTime() << ')';

			return ss.str();
		}
	);

	std::copy(std::cbegin(strs), std::cend(strs), std::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}
