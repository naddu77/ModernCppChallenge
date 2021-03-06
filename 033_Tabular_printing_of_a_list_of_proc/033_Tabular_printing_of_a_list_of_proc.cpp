// 033_Tabular_printing_of_a_list_of_proc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 자동정렬을 이용하고 중복을 허용하기 위해 multi_set이용
// - enum class의 요소를 문자열로 변환하는 table을 자동 생성하기 위해 Preprocessor Metaprogramming 이용
// Question
// - 요소가 많은 구조체의 경우 operator< 구현의 최선은?

#include "pch.h"
#include <boost/preprocessor.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <numeric>
#include <set>
#include <map>
#include <sstream>
#include <string_view>

#define MAKE_PAIR(r, data, elem) BOOST_PP_CAT(BOOST_PP_CAT({ BOOST_PP_CAT(data, ::##elem), BOOST_PP_COMMA()), BOOST_PP_STRINGIZE(elem) } BOOST_PP_COMMA())

#define MAKE_PAIRS3(...)	BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)
#define MAKE_PAIRS2(Class, ...)	BOOST_PP_SEQ_FOR_EACH(MAKE_PAIR, Class, MAKE_PAIRS3(__VA_ARGS__))
#define MAKE_PAIRS(Class, ...)	BOOST_PP_SEQ_ENUM(MAKE_PAIRS2(Class, __VA_ARGS__))

//#define MakeEnumClass(Class, ...)	\
//	enum class Class \
//	{	\
//		__VA_ARGS__	\
//	};	\
//	\
//	std::map<Class, std::string> Class##_to_string_map	\
//	{	\
//		MAKE_PAIRS(Class, __VA_ARGS__)	\
//	}

#define DeclareEnumClass(Name, ...)	\
	enum class Name \
	{	\
		__VA_ARGS__	\
	};	\
	\
	static std::map<Name, std::string> Name##_to_string_map

#define ImplementEnumClass(Class, Name, ...)	\
	std::map<Class::Name, std::string> Class::Name##_to_string_map	\
	{	\
		MAKE_PAIRS2(Class::Name, __VA_ARGS__)	\
	}

class ProcInfo
{
public:
	DeclareEnumClass(ProcStatus,
		SUSPENDED,
		RUNNING
	);
	DeclareEnumClass(Platforms,
		P32BIT,
		P64BIT
	);

	ProcInfo(int const id, std::string const& name, ProcStatus const status,
		std::string const& account, std::size_t const memory, Platforms const platform)
		: id(id), name(name), status(status), account(account), memory(memory), platform(platform)
	{

	}

	~ProcInfo() = default;

	bool operator<(ProcInfo const& other) const
	{
		if (name == other.name)
			if (id == other.id)
				if (status == other.status)
					if (account == other.account)
						if (memory == other.memory)
							if (platform == other.platform)
								return false;
							else
								return Platforms_to_string_map[platform] < Platforms_to_string_map[other.platform];
						else
							return memory < other.memory;
					else
						return account < other.account;
				else
					return ProcStatus_to_string_map[status] < ProcStatus_to_string_map[other.status];
			else
				return id < other.id;
		else
			return name < other.name;
	}

	std::vector<std::string> ToStrings(void) const
	{
		return {
			name,
			std::to_string(id),
			ProcStatus_to_string_map[status],
			account,
			std::to_string(memory / 1024),
			Platforms_to_string_map[platform]
		};
	}

private:
	int         id;
	std::string name;
	ProcStatus  status;
	std::string account;
	std::size_t memory;
	Platforms   platform;
};

ImplementEnumClass(ProcInfo, ProcStatus,
	SUSPENDED,
	RUNNING
	);

ImplementEnumClass(ProcInfo, Platforms,
	P32BIT,
	P64BIT
);

class ProcInfoManager
{
public:
	ProcInfoManager& Add(ProcInfo&& proc_info) &
	{
		l.insert(std::move(proc_info));

		return *this;
	}

	ProcInfoManager&& Add(ProcInfo&& proc_info) &&
	{
		Add(std::move(proc_info));

		return std::move(*this);
	}

	template <typename... Ts>
	ProcInfoManager& Add(Ts&&... ts) &
	{
		l.emplace(std::forward<Ts>(ts)...);

		return *this;
	}

	template <typename... Ts>
	ProcInfoManager&& Add(Ts&&... ts) &&
	{
		Add(std::forward<Ts>(ts)...);

		return std::move(*this);
	}

	void Print(void) const &
	{
		auto const proc_info_list = GetProcInfoList();
		auto const str_max_list = GetColumnMaxLengthList(proc_info_list);

		auto const length = 4 + (std::size(str_max_list) - 1) * 3 + std::reduce(std::cbegin(str_max_list), std::cend(str_max_list));

		PrintTitle("Process Information", length);
		std::cout.fill(' ');

		auto const result = std::reduce(std::cbegin(proc_info_list), std::cend(proc_info_list), std::vector<std::string>{},
			[str_max_list](auto acc, auto const& e) {
				std::stringstream ss;

				ss << "  "
					<< std::left << std::setw(str_max_list[0]) << e[0] << "   "
					<< std::left << std::setw(str_max_list[1]) << e[1] << "   "
					<< std::left << std::setw(str_max_list[2]) << e[2] << "   "
					<< std::left << std::setw(str_max_list[3]) << e[3] << "   "
					<< std::right << std::setw(str_max_list[4]) << e[4] << "   "
					<< std::left << std::setw(str_max_list[5]) << e[5] << "   ";

				acc.push_back(ss.str());

				return std::move(acc);
			}
		);

		std::copy(std::cbegin(result), std::cend(result), std::ostream_iterator<std::string>(std::cout, "\n"));
		PrintLine('=', length);
	}

	ProcInfoManager&& Print(void) &&
	{
		Print();

		return std::move(*this);
	}

private:
	std::multiset<ProcInfo> l;

	std::vector<std::vector<std::string>> GetProcInfoList(void) const
	{
		std::vector<std::vector<std::string>> proc_info_list;

		std::transform(std::cbegin(l), std::cend(l), std::back_inserter(proc_info_list), [](auto const& e) {
			return e.ToStrings();
		});

		return proc_info_list;
	}

	std::vector<std::size_t> GetColumnMaxLengthList(std::vector<std::vector<std::string>> const& proc_info_list) const
	{
		return std::reduce(std::cbegin(proc_info_list), std::cend(proc_info_list), std::vector<std::size_t>(std::size(proc_info_list.front()), 0u), [](auto acc, auto const e) {
			std::transform(std::cbegin(e), std::cend(e), std::begin(acc), std::begin(acc), [](auto const a, auto const b) {
				return std::max(std::size(a), b);
			});

			return std::move(acc);
		});
	}

	void PrintTitle(std::string_view const title, std::size_t const space) const
	{
		PrintLine('=', space);
		std::fill_n(std::ostream_iterator<char>(std::cout), (space - std::size(title)) >> 1, ' ');
		std::cout << title << std::endl;
		PrintLine('=', space);
	}

	void PrintLine(char const c, std::size_t const count) const
	{
		std::fill_n(std::ostream_iterator<char>(std::cout), count, c);
		std::cout << std::endl;
	}
};

int main()
{
	using namespace std::string_literals;

	ProcInfoManager{}
		.Add(512, "cmd.exe"s, ProcInfo::ProcStatus::RUNNING, "SYSTEM"s, 148293, ProcInfo::Platforms::P64BIT)
		.Add(1044, "chrome.exe"s, ProcInfo::ProcStatus::RUNNING, "marius.bancila"s, 25180454, ProcInfo::Platforms::P32BIT)
		.Add(7108, "explorer.exe"s, ProcInfo::ProcStatus::RUNNING, "marius.bancila"s, 2952943, ProcInfo::Platforms::P64BIT)
		.Add(10100, "chrome.exe"s, ProcInfo::ProcStatus::RUNNING, "marius.bancila"s, 227756123, ProcInfo::Platforms::P32BIT)
		.Add(22456, "skype.exe"s, ProcInfo::ProcStatus::SUSPENDED, "marius.bancila"s, 16870123, ProcInfo::Platforms::P64BIT)
		.Print();

	return 0;
}