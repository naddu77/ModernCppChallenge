// 019_Adding_a_range_of_values_to_a_container.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <vector>
#include <iostream>
#include <iterator>

template <typename Cont, typename... Ts,
	typename = typename Cont::value_type
>
void PushBack(Cont& cont, Ts&&... args)
{
	(cont.push_back(std::forward<Ts>(args)), ...);
}

//template <template <typename...> typename Cont, typename... Elem, typename... Ts>
//void PushBack(Cont<Elem...>& cont, Ts&&... args)
//{
//	(cont.push_back(std::forward<Ts>(args)), ...);
//}

int main()
{
	std::vector<int> l{ 1,2,3,4,5 };

	PushBack(l, 6, 7, 8, 9, 10);

	std::copy(std::cbegin(l), std::cend(l), std::ostream_iterator<typename decltype(l)::value_type>(std::cout, " "));
	std::cout << std::endl;

	return 0;
}