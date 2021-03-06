// 018_Minimum_function_with_any_number_of_arguments.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: SFINAE를 이용함

#include "pch.h"
#include <iostream>
#include <type_traits>
#include <algorithm>

template <typename T1, typename T2>
std::common_type_t<T1, T2> Minimum(T1 a, T2 b)
{
	return b < a ? b : a;
}

template <typename T, typename... Ts>
std::common_type_t<T, Ts...> Minimum(T t, Ts... ts)
{
	return Minimum(t, Minimum(ts...));
}

template <typename T1, typename T2, typename Comp,
	typename = std::enable_if_t<std::is_invocable_v<Comp, T1, T2>>
>
std::common_type_t<T1, T2> Minimum(Comp comp, T1 a, T2 b)
{
	return comp(a, b) ? a : b;
}

template <typename T, typename... Ts, typename Comp,
	typename = std::enable_if_t<std::is_invocable_v<Comp, T, T>>
>
std::common_type_t<T, Ts...> Minimum(Comp comp, T t, Ts... ts)
{
	return Minimum(comp, t, Minimum(comp, ts...));
}

bool Comp(int a, int b)
{
	return a < b;
}

int main()
{
	std::cout << Minimum(1, 2.0, 3) << std::endl;
	std::cout << Minimum(2, 1, 3) << std::endl;
	std::cout << Minimum([](auto const a, auto const b) { return a < b; }, 1, 2.0, 3) << std::endl;
	std::cout << Minimum([](auto const a, auto const b) { return a < b; }, 2, 1, 3) << std::endl;
	std::cout << Minimum(std::less<int>{}, 2, 1, 3) << std::endl;
	std::cout << Minimum(std::greater<int>{}, 2, 1, 3) << std::endl;
	std::cout << Minimum(&Comp, 2, 1, 3) << std::endl;

	return 0;
}
