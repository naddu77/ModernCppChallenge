// 014_Validating_ISBNs.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <numeric>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory>
#include <map>

class ISBN_Validater
{
public:
	virtual bool Validate(std::string_view const& isbn) const = 0;

	inline bool AllDigits(std::string_view const& isbn) const
	{
		return std::all_of(std::cbegin(isbn), std::cend(isbn), std::isdigit);
	}
};

class ISBN10_Validater : public ISBN_Validater
{
public:
	virtual bool Validate(std::string_view const& isbn) const override
	{
		auto w = 10;

		return AllDigits(isbn) && std::reduce(std::cbegin(isbn), std::cend(isbn), 0, [&w](auto const acc, auto const c) {
			return acc + w-- * (c - '0');
		}) % 11 == 0;
	}
};

class ISBN13_Validater : public ISBN_Validater
{
public:
	virtual bool Validate(std::string_view const& isbn) const override
	{
		auto i = 1;

		return AllDigits(isbn) && std::reduce(std::cbegin(isbn), std::cend(isbn), 0, [&i](auto const acc, auto const c) {
			return acc + (i++ % 2 == 1 ? c - '0' : 3 * (c - '0'));
		}) % 10 == 0;
	}
};

class ISBN
{
public:
	enum class ISBN_Type
	{
		ISBN10,
		ISBN13
	};

	static bool Validate(std::string const& isbn)
	{
		auto const refined_isbn = RefineISBN(isbn);

		return isbn_validaters[GetISBN_Type(refined_isbn)]->Validate(refined_isbn);
	}

private:
	static std::map<ISBN::ISBN_Type, std::shared_ptr<ISBN_Validater>> isbn_validaters;

	inline static std::string RefineISBN(std::string isbn)
	{
		return std::string(std::begin(isbn), std::remove(std::begin(isbn), std::end(isbn), '-'));
	}

	static ISBN_Type GetISBN_Type(std::string const& isbn)
	{
		switch (std::size(isbn))
		{
		case 10:
			return ISBN_Type::ISBN10;

		case 13:
			return ISBN_Type::ISBN13;

		default:
			throw std::invalid_argument("Unknown ISBN type.");
		}
	}
};

std::map<ISBN::ISBN_Type, std::shared_ptr<ISBN_Validater>> ISBN::isbn_validaters{
	{ ISBN::ISBN_Type::ISBN10, std::make_shared<ISBN10_Validater>() },
	{ ISBN::ISBN_Type::ISBN13, std::make_shared<ISBN13_Validater>() }
};

int main()
{
	std::cout << ISBN::Validate("3161484100") << std::endl;
	std::cout << ISBN::Validate("1413304540") << std::endl;
	std::cout << ISBN::Validate("9781413304541") << std::endl;
	std::cout << ISBN::Validate("1-4133-0454-0") << std::endl;
	std::cout << ISBN::Validate("978-1-4133-0454-1") << std::endl;

	return 0;
}