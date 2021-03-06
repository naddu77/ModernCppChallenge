// 020_Container_any,_all,_none.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <vector>
#include <array>
#include <list>
#include <cassert>
#include <algorithm>

template <typename Cont, typename T>
bool AnyOf(Cont const& cont, T const& t)
{
	return std::any_of(std::cbegin(cont), std::cend(cont), [t](auto const& e) { return t == e; });
}

template <typename Cont, typename... Ts>
bool contains_any(Cont const& cont, Ts const&... args)
{
	return (... || AnyOf(cont, args));
}

template <typename Cont, typename... Ts>
bool contains_all(Cont const& cont, Ts const&... args)
{
	return (... && AnyOf(cont, args));
}

template <typename Cont, typename... Ts>
bool contains_none(Cont const& cont, Ts const&... args)
{
	return !contains_any(cont, args...);
}

int main()
{
	std::vector<int> v{ 1, 2, 3, 4, 5, 6 };
	assert(contains_any(v, 0, 3, 30));

	std::array<int, 6> a{ { 1, 2, 3, 4, 5, 6 } };
	assert(contains_all(a, 1, 3, 5, 6));

	std::list<int> l{ 1, 2, 3, 4, 5, 6 };
	assert(!contains_none(l, 0, 6));

	return 0;
}

