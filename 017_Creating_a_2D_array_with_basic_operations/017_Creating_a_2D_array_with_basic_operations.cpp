// 017_Creating_a_2D_array_with_basic_operations.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: 버퍼를 std::array<std::array<T, C>, R>를 이용함.

#include "pch.h"
#include <array>
#include <optional>
#include <iostream>
#include <iterator>
#include <string_view>
#include <string>

template <typename T, std::size_t R, std::size_t C>
class Array2D
{
	using Position = std::pair<std::size_t, std::size_t>;

public:
	Array2D() = default;

	Array2D(std::initializer_list<T> l)
	{
		Position pos{ 0u, 0u };

		for (auto&& e : l)
		{
			if (pos < Position{ R, 0u })
			{
				auto const[r, c] = pos;

				buf[r][c] = std::move(e);

				pos = GetNext(pos);
			}
			else
				break;
		}
	}

	Array2D(Array2D const& other)
	{
		std::copy(std::cbegin(other.buf), std::cend(other.buf), std::back_inserter(other.buf));
	}

	Array2D(Array2D&& other)
		: buf(std::move(other.buf))
	{

	}

	auto Size(std::size_t const rank) const
	{
		switch (rank)
		{
		case 1:
			return std::size(buf);

		case 2:
			return std::size(buf.front());

		default:
			throw std::invalid_argument("1 또는 2만 입력 가능!");
		}
	}

	T& operator()(std::size_t const r, std::size_t const c)
	{
		return buf[r][c];
	}

	T& At(std::size_t const r, std::size_t const c)
	{
		return buf.at(r).at(c);
	}

	T const& At(std::size_t const r, std::size_t const c) const
	{
		return buf.at(r).at(c);
	}


	std::array<T, C>& operator[](std::size_t const r)
	{
		return buf[r];
	}

	std::array<T, C> const& operator[](std::size_t const r) const
	{
		return buf[r];
	}

	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = int;
		using difference_type = ptrdiff_t;
		using pointer = int*;
		using reference = int&;

		Iterator(std::array<std::array<T, C>, R>& buf, bool is_start = true)
			: buf(buf)
		{
			if (is_start)
				pos = std::make_pair(0u, 0u);
			else
				pos = std::make_pair(R, 0u);
		}

		Iterator(Iterator const& iter)
			: buf(iter.buf), pos(iter.pos)
		{

		}

		Iterator& operator++()
		{
			pos = GetNext(pos);

			return *this;
		}

		Iterator operator++(int)
		{
			Iterator it(*this);

			pos = GetNext(pos);

			return it;
		}

		T& operator*()
		{
			return buf[pos.first][pos.second];
		}

		T const& operator*() const
		{
			return buf[pos.first][pos.second];
		}

		bool operator!=(Iterator const& rhs) const
		{
			return pos != rhs.pos;
		}

		bool operator==(Iterator const& rhs) const
		{
			return !(*this != rhs);
		}

	private:
		std::array<std::array<T, C>, R>& buf;
		Position pos;

		Position GetNext(Position p) const
		{
			auto&[r, c] = p;

			if (c < C - 1)
				++c;
			else if (r < R - 1)
			{
				++r;
				c = 0;
			}
			else
				return { R, 0u };

			return p;
		}
	};

	Iterator begin()
	{
		return Iterator(buf);
	}

	Iterator end()
	{
		return Iterator(buf, false);
	}

	void Fill(T const t)
	{
		std::fill(std::begin(*this), std::end(*this), t);
	}

	void Swap(Array2D&& other) noexcept
	{
		std::swap(std::move(*this), std::move(other));
	}

	void Swap(Array2D& other) noexcept
	{
		std::swap(this->buf, other.buf);
	}

private:
	std::array<std::array<T, C>, R> buf;

	Position GetNext(Position p) const
	{
		auto&[r, c] = p;

		if (c < C - 1)
			++c;
		else if (r < R - 1)
		{
			++r;
			c = 0;
		}
		else
			return { R, 0u };

		return p;
	}
};

template <typename T, std::size_t R, std::size_t C>
void PrintArray2D(std::string const& title, Array2D<T, R, C>& arr)
{
	std::cout << title << ": ";
	std::copy(std::begin(arr), std::end(arr), std::ostream_iterator<T>(std::cout, " "));
	std::cout << std::endl;
}

int main()
{
	// element access
	Array2D<int, 2, 3> a{ 1, 2, 3, 4, 5, 6 };
	for (size_t i = 0; i < a.Size(1); ++i)
		for (size_t j = 0; j < a.Size(2); ++j)
			a(i, j) *= 2;

	// iterating
	PrintArray2D("iterating", a);

	// filling 
	Array2D<int, 2, 3> b;

	b.Fill(1);
	PrintArray2D("filling", b);

	// swapping
	a.Swap(b);
	PrintArray2D("a", a);
	PrintArray2D("b", b);

	// moving
	Array2D<int, 2, 3> c(std::move(b));

	PrintArray2D("b", b);
	PrintArray2D("c", c);

	std::cout << c.At(1, 0) << std::endl;
	std::cout << c[1][0] << std::endl;

	return 0;
}
