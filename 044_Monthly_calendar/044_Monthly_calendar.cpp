// 044_Monthly_calendar.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::date_time library 이용
// - Haskell 스럽게 구현


//import Data.Time.Calendar
//import Data.Time.Calendar.WeekDate
//import Text.Printf
//import Data.List.Split
//import Data.Char
//
//main = do
//putStr "Year: "
//year_str < -getLine
//	putStr "Month: "
//	month_str < -getLine
//	let year = read year_str::Integer
//	let month = read month_str::Int
//	let month_length = gregorianMonthLength year month
//	let week = getWeek year month 1
//	let day_list = chunksOf 7 $ replicate week "    " ++map(\x->replicate(3 - (length $ show x)) ' ' ++show x++ " ")[1..month_length]
//	let day_list_str = map concat day_list
//	showCalendar day_list_str
//
//	getWeek year month day = digitToInt.last.showWeekDate $ fromGregorian year month day
//
//	showCalendar xss = do
//	putStrLn "Mon Tue Wed Thu Fri Sat Sun"
//	mapM_ putStrLn xss

// ChunksOf in Haskell
//-- | Standard build function, specialized to building lists.
//--
//--   Usually build is given the rank-2 type
//--
//--   > build :: (forall b. (a -> b -> b) -> b -> b) -> [a]
//--
//--   but since we only use it when @(b ~ [a])@, we give it the more
//--   restricted type signature in order to avoid needing a
//--   non-Haskell2010 extension.
//--
//--   Note that the 0.1.4.3 release of this package did away with a
//--   custom @build@ implementation in favor of importing one from
//--   "GHC.Exts", which was (reportedly) faster for some applications.
//--   However, in the interest of simplicity and complete Haskell2010
//--   compliance as @split@ is being included in the Haskel Platform,
//--   version 0.2.1.0 has gone back to defining @build@ manually.  This
//--   is in line with @split@'s design philosophy of having efficiency
//--   as a non-goal.
//build :: ((a -> [a] -> [a]) -> [a] -> [a]) -> [a]
//build g = g (:) []
//
//-- | @'chunksOf' n@ splits a list into length-n pieces.  The last
//--   piece will be shorter if @n@ does not evenly divide the length of
//--   the list.  If @n <= 0@, @'chunksOf' n l@ returns an infinite list
//--   of empty lists.  For example:
//--
//--   Note that @'chunksOf' n []@ is @[]@, not @[[]]@.  This is
//--   intentional, and is consistent with a recursive definition of
//--   'chunksOf'; it satisfies the property that
//--
//--   @chunksOf n xs ++ chunksOf n ys == chunksOf n (xs ++ ys)@
//--
//--   whenever @n@ evenly divides the length of @xs@.
//chunksOf :: Int -> [e] -> [[e]]
//chunksOf i ls = map (take i) (build (splitter ls)) where
//  splitter :: [e] -> ([e] -> a -> a) -> a -> a
//  splitter [] _ n = n
//  splitter l c n  = l `c` splitter (drop i l) c n



#include "pch.h"
#include <boost/date_time.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <deque>

using namespace boost::gregorian;
using namespace std::string_literals;

template <typename T>
auto Take(std::size_t const n, std::deque<T> l)
{
	return n > std::size(l) ? l : std::deque<T>(std::begin(l), std::begin(l) + n);
};

template <typename T>
auto Drop(std::size_t const n, std::deque<T> l)
{
	return n > std::size(l) ? std::deque<T>{} : std::deque<T>(std::begin(l) + n, std::end(l));
};

//  splitter :: [e] -> ([e] -> a -> a) -> a -> a
//  splitter [] _ n = n
//  splitter l c n  = l `c` splitter (drop i l) c n
template <typename T>
std::deque<std::deque<T>> SplitterImpl(std::size_t const i, std::deque<T> const& l)
{
	return std::empty(l) ? std::deque<std::deque<T>>{} : l + SplitterImpl(i, Drop(i, l));
};

//chunksOf :: Int -> [e] -> [[e]]
//chunksOf i ls = map (take i) (build (splitter ls))
template <typename T>
std::deque<std::deque<T>> ChunksOf(std::size_t const i, std::deque<T> const& ls)
{
	auto Splitter = std::bind(&SplitterImpl<T>, i, std::placeholders::_1);
	auto days_2d = Splitter(ls);

	std::transform(std::begin(days_2d), std::end(days_2d), std::begin(days_2d), std::bind(&Take<std::string>, i, std::placeholders::_1));

	return days_2d;
}

template <typename T>
std::deque<std::deque<T>> operator+(std::deque<T> l1, std::deque<std::deque<T>> l2)
{
	l2.push_front(std::move(l1));

	return std::move(l2);
}

template <typename F, typename U>
decltype(auto) operator|(F&& f, U&& u)
{
	return std::invoke(std::forward<U>(u), std::forward<F>(f));
}

int main()
{
	int year;
	int month;

	std::cout << "[Year] [Month]: ";
	std::cin >> year >> month;

	date const d(year, month, 1);

	// date -> [int]
	auto Generate = [](auto const& d) {
		std::deque<int> days(d.end_of_month().day());

		std::iota(std::begin(days), std::end(days), 1);

		return days;
	};

	// [int] -> [string]
	auto Map = [week = d.day_of_week()](auto const& days) {
		std::deque<std::string> days_1d(week - 1, "    ");

		std::transform(std::cbegin(days), std::cend(days), std::back_inserter(days_1d), [](auto const d) {
			std::stringstream ss;

			ss.fill(' ');
			ss << std::setw(3) << d << ' ';

			return ss.str();
		});

		return days_1d;
	};

	// [string] -> [[string]]
	auto Chunks = [](auto const& days_1d) {
		return ChunksOf(7, days_1d);
	};

	// [[string]] -> [string]
	auto Reduce = [](auto const& days_2d) {
		return std::accumulate(std::cbegin(days_2d), std::cend(days_2d), std::vector<std::string>{}, [](auto acc, auto const& e) {
			std::stringstream ss;

			std::copy(std::cbegin(e), std::cend(e), std::ostream_iterator<std::string>(ss));
			acc.push_back(ss.str());

			return std::move(acc);
		});
	};

	// [string] -> IO ()
	auto PrintCalendar = [](auto const& result) {
		std::cout << "\nMon Tue Wed Thu Fri Sat Sun\n";
		std::copy(std::cbegin(result), std::cend(result), std::ostream_iterator<std::string>(std::cout, "\n"));
	};

	Generate(d) | Map | Chunks | Reduce | PrintCalendar;	// Like Range

	return 0;
}