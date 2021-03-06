// 039_Measuring_function_execution_time.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 교재 코드는 리턴 값을 버림, 리턴 값이 필요한 함수에는 적용이 어려움
// - void 리턴인 함수에 대한 예외 처리가 필요함
// - 함수로 만들면 시간 측정 값을 리턴 값으로 얻어 오려면 원래의 리턴 형식을 변형하는 수밖에 없음.
// - 그래서 클래스를 이용

#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

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

//template <typename Unit = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock, typename Func, typename... Ts>
//void Test(Func&& func, Ts&&... args)
//{
//	FunctionExecutionTimeCalculator function_execution_time_calculator;
//
//	if constexpr (std::is_same_v<std::invoke_result_t<Func, Ts...>, void>)
//	{
//		function_execution_time_calculator.operator()<Unit, Clock>(std::forward<Func>(func), std::forward<Ts>(args)...);
//		std::cout << "Nothing (" << function_execution_time_calculator.GetFunctionExecutionTime() << ")" << std::endl;
//	}
//	else
//	{
//		std::cout << function_execution_time_calculator.operator()<Unit, Clock>(std::forward<Func>(func), std::forward<Ts>(args)...) << " (" << function_execution_time_calculator.GetFunctionExecutionTime() << ")" << std::endl;
//	}
//}

template <typename Unit = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock, typename Func, typename... Ts>
std::enable_if_t<std::is_same_v<std::invoke_result_t<Func, Ts...>, void>> Test(Func&& func, Ts&&... args)
{
	FunctionExecutionTimeCalculator function_execution_time_calculator;

	function_execution_time_calculator.operator()<Unit, Clock>(std::forward<Func>(func), std::forward<Ts>(args)...);
	std::cout << "Nothing (" << function_execution_time_calculator.GetFunctionExecutionTime() << ")" << std::endl;
}

template <typename Unit = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock, typename Func, typename... Ts>
std::enable_if_t<!std::is_same_v<std::invoke_result_t<Func, Ts...>, void>> Test(Func&& func, Ts&&... args)
{
	FunctionExecutionTimeCalculator function_execution_time_calculator;

	std::cout << function_execution_time_calculator.operator()<Unit, Clock>(std::forward<Func>(func), std::forward<Ts>(args)...) << " (" << function_execution_time_calculator.GetFunctionExecutionTime() << ")" << std::endl;
}

int Foo(void)
{
	std::this_thread::sleep_for(2s);

	return 1;
}

void Goo(void)
{
	std::this_thread::sleep_for(1s);
}

int Add(int const a, int const b)
{
	std::this_thread::sleep_for(500ms);

	return a + b;
}

template <typename T1, typename T2>
std::common_type_t<T1, T2> Max(T1 const& t1, T2 const& t2)
{
	std::this_thread::sleep_for(100ms);

	return t1 < t2 ? t1 : t2;
}

class FunctionObject
{
public:
	int operator()(int const a, int const b) const
	{
		std::this_thread::sleep_for(100ms);

		return a + b;
	}
};

class GenerincFunctionObject
{
public:
	template <typename T>
	T operator()(T const a, T const b) const
	{
		std::this_thread::sleep_for(100ms);

		return a + b;
	}
};

int main()
{
	auto LambdaAdd = [](auto const a, auto const b) {
		std::this_thread::sleep_for(100ms);

		return a + b;
	};

	std::function<int(int const)> std_function_add = std::bind(&Add, 10, std::placeholders::_1);

	Test(&Foo);
	Test(&Goo);
	Test(&Add, 1, 2);
	Test(Max<int, int>, 1, 2);	// 추론해서 넘기는 법을 모르겠음
	Test(LambdaAdd, 3, 4);
	Test(FunctionObject(), 9, 8);
	Test(GenerincFunctionObject(), 7, 8);
	Test(std_function_add, 3);
	Test(std::bind(LambdaAdd, std::placeholders::_1, 15), 25);

	return 0;
}