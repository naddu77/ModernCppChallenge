// 016_Enumerating_IPv4_addresses_in_a_range.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <string_view>
#include <regex>
#include <numeric>
#include <optional>
#include <algorithm>
#include <array>
#include <utility>

using namespace std::rel_ops;

class IPv4
{
	friend std::ostream& operator<<(std::ostream& os, IPv4 const& ipv4)
	{
		return os << ipv4.ipv4_address.ToString();
	}

	friend std::istream& operator>>(std::istream& is, IPv4& ipv4)
	{
		std::string input;

		is >> input;

		ipv4.ReadUsingRegularExpression(input);

		return is;
	}

public:
	IPv4(unsigned long const ipv4_address = 0)
	{
		this->ipv4_address.ulong = ipv4_address;
	}

	IPv4(unsigned char const first, unsigned char const second, unsigned char const third, unsigned char const fourth)
	{
		ipv4_address.arr[3] = first;
		ipv4_address.arr[2] = second;
		ipv4_address.arr[1] = third;
		ipv4_address.arr[0] = fourth;
	}

	IPv4(std::initializer_list<unsigned char> ipv4_address)
	{
		if (std::size(ipv4_address) < sizeof(this->ipv4_address.arr))
			throw std::invalid_argument("4보다 커야 함.");

		std::reverse_copy(std::cbegin(ipv4_address), std::cbegin(ipv4_address) + sizeof(this->ipv4_address.arr), std::begin(this->ipv4_address.arr));
	}

	IPv4(std::string const& ipv4_address)
	{
		ReadUsingRegularExpression(ipv4_address);
	}

	IPv4& operator++()
	{
		++ipv4_address.ulong;

		return *this;
	}

	IPv4 const operator++(int)
	{
		IPv4 result(*this);

		++(*this);

		return result;
	}

	bool operator==(IPv4 const& rhs) const
	{
		return this->ToUnsignedLong() == rhs.ToUnsignedLong();
	}

	bool operator<(IPv4 const& rhs) const
	{
		return this->ToUnsignedLong() < rhs.ToUnsignedLong();
	}

	unsigned long ToUnsignedLong(void) const
	{
		return ipv4_address.ulong;
	}

	std::string ToString(void) const
	{
		return ipv4_address.ToString();
	}

private:
	using IPv4_Address = unsigned long;

	union
	{
		IPv4_Address ulong;
		std::array<unsigned char, sizeof(IPv4_Address)> arr;

		void SetIPv4_UnsignedLong(IPv4_Address const ipv4_address_ulong)
		{
			this->ulong = ipv4_address_ulong;
		}

		std::string ToString(void) const
		{
			std::string result;

			result.push_back(arr[3]);
			result.push_back('.');
			result.push_back(arr[2]);
			result.push_back('.');
			result.push_back(arr[1]);
			result.push_back('.');
			result.push_back(arr[0]);

			return std::to_string(arr[3]) + '.' + std::to_string(arr[2]) + '.' + std::to_string(arr[1]) + '.' + std::to_string(arr[0]);
		}
	} ipv4_address;

	bool ReadUsingRegularExpression(std::string const& str)
	{
		std::regex ipv4_style(R"~(\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)~");
		std::smatch tokens;

		if (std::regex_match(str, tokens, ipv4_style))
		{
			this->ipv4_address.ulong = std::reduce(std::next(std::cbegin(tokens)), std::cend(tokens), 0ul, [](auto const acc, auto const token) {
				return (acc << 8ul) + std::stoul(token);
			});
		}
		else
			return false;

		return true;
	}
};

int main()
{
	std::cout << "input range: ";

	IPv4 a1, a2;

	std::cin >> a1 >> a2;

	if (a2 > a1)
	{
		for (IPv4 a = a1; a <= a2; a++)
		{
			std::cout << a << std::endl;
		}
	}
	else
	{
		std::cerr << "invalid range!" << std::endl;
	}
}