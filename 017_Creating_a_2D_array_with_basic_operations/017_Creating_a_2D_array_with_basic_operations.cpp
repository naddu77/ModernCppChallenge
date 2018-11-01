// 017_Creating_a_2D_array_with_basic_operations.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <array>

template <typename T, std::size_t R, std::size_t C>
class Array2D
{
public:
	Array2D(std::initializer_list<T> l)
	{
		auto r = 0u;
		auto c = 0u;

		for (auto&& e : l)
		{
			buf[r][c] = std::move(e);
			++c;

			if (c >= C)
			{
				c = 0u;
				++r;
			}

			if (r >= R)
				break;
		}

	}

	std::size_t Size(std::size_t const rank) const
	{
		switch (rank)
		{
		case 1:
			return buf.size();
			
		case 2:
			return buf.front().size();

		default:
			throw std::out_of_range("Rank is out of range!");
		}
	}

	T& operator()(std::size_t const r, std::size_t const c)
	{
		return buf[r][c];
	}

private:
	std::array<std::array<T, C>, R> buf;
};

int main()
{
	// element access
	Array2D<int, 2, 3> a{ 1, 2, 3, 4, 5, 6 };

	for (size_t i = 0; i < a.Size(1); ++i)
		for (size_t j = 0; j < a.Size(2); ++j)
			a(i, j) *= 2;

	//// iterating
	//std::copy(std::begin(a), std::end(a),
	//	std::ostream_iterator<int>(std::cout, " "));

	//// filling 
	//Array2D<int, 2, 3> b;
	//b.fill(1);

	//// swapping
	//a.swap(b);

	//// moving
	//Array2D<int, 2, 3> c(std::move(b));
}