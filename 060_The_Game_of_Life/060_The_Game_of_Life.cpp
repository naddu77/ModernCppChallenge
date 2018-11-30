// 060_The_Game_of_Life.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Range-v3 이용
// - 좀 더 간결?

#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <chrono>
#include <thread>
#include <iterator>

namespace std
{
	template <typename T>
	std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
	{
		std::copy(std::cbegin(v), std::cend(v), std::ostream_iterator<T>(os));

		return os;
	}
}

class Universe
{
public:
	enum class Seed
	{
		RANDOM,
		TEN_CELL_ROW
	};

	Universe(size_t const width, size_t const height) :
		rows(height), columns(width), grid(rows, std::vector<char>(columns)), dist(0, 4)
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		mt.seed(seq);
	}

	void Run(Seed const s, int const generations,
		std::chrono::milliseconds const ms =
		std::chrono::milliseconds(100))
	{
		Reset();
		Initialize(s);
		Display();

		auto i = 0;

		do
		{
			NextGeneration();
			Display();

			std::this_thread::sleep_for(ms);
		} while (i++ < generations || generations == 0);
	}

private:
	size_t rows;
	size_t columns;

	using Grid = std::vector<std::vector<char>>;
	Grid grid;
	const char alive = '*';
	const char dead = ' ';

	std::uniform_int_distribution<> dist;
	std::mt19937 mt;

	Universe() = delete;

	void NextGeneration()
	{
		auto input = ranges::view::iota(0, rows) >>= [this](auto const r) { return
			ranges::view::iota(0, columns) >>= [=](auto const c) { return
			ranges::yield(std::pair{ r, c });
		}; };

		Grid newgrid(rows, std::vector<char>(columns));

		for (auto const&[r, c] : input)
		{
			auto count = CountNeighbors(r, c);

			newgrid[r][c] = (grid[r][c] == alive ?
				((count == 2 || count == 3) ? alive : dead) :
				((count == 3) ? alive : dead));
		}

		grid.swap(newgrid);
	}

	void ResetDisplay()
	{
		system("cls");
	}

	void Display()
	{
		ResetDisplay();
		std::copy(std::cbegin(grid), std::cend(grid), std::ostream_iterator<std::vector<char>>(std::cout, "\n"));
	}

	void Initialize(Seed const s)
	{
		if (s == Seed::TEN_CELL_ROW)
		{
			for (size_t c = columns / 2 - 5; c < columns / 2 + 5; c++)
				grid[rows / 2][c] = alive;
		}
		else
		{
			ranges::transform(grid, std::begin(grid), [this](auto&& rng) {
				ranges::transform(rng, std::begin(rng), [this](auto&&) {
					return dist(mt) == 0 ? alive : dead;
				});

				return std::move(rng);
			});
		}
	}

	void Reset()
	{
		std::fill(std::begin(grid), std::end(grid), std::vector<char>(columns, dead));
	}

	std::size_t CountNeighbors(size_t const row, size_t const col)
	{
		auto directions = ranges::view::iota(-1, 2) >>= [](int const r) { return
			ranges::view::iota(-1, 2) >>= [=](int const c) { return
			ranges::yield_if(r != 0 || c != 0, std::pair{ r, c });
		}; };

		return ranges::count(directions
			| ranges::view::filter([row, col, this](auto const& p) { return row + p.first < rows && col + p.second < columns; })
			| ranges::view::transform([row, col, this](auto const& p) { return grid[row + p.first][col + p.second]; }),
			alive);
	}
};	

int main()
{
	using namespace std::chrono_literals;

	Universe u(50, 20);
	u.Run(Universe::Seed::RANDOM, 100, 100ms);

	return 0;
}