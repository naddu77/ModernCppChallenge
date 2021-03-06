// 022_Literals_of_various_temperature.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: std::chrono 형식으로 구현하지 않았음. 온도 환산식을 Table로 관리하게 구현.

#include "pch.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <utility>
#include <cassert>

using namespace std::rel_ops;

class Temperature
{
public:
	enum Type
	{
		Celsius,
		Fahrenheit,
		Kelvin
	};

	Temperature(long double const& t, Type const type)
		: t(t), type(type)
	{

	}

	bool operator==(Temperature const& other) const
	{
		return std::fabsl(t - convesion_table[other.type][type](other.t)) < std::numeric_limits<long double>::epsilon();
	}

	bool operator<(Temperature const& other) const
	{
		return t < convesion_table[other.type][type](other.t);
	}

	Temperature operator+(Temperature const& other) const
	{
		return Temperature(t + convesion_table[other.type][type](other.t), type);
	}

	Temperature operator-(Temperature const& other) const
	{
		return Temperature(t - convesion_table[other.type][type](other.t), type);
	}

	Temperature& operator+=(Temperature const& other)
	{
		t += convesion_table[other.type][type](other.t);

		return *this;
	}

	Temperature& operator-=(Temperature const& other)
	{
		t -= convesion_table[other.type][type](other.t);

		return *this;
	}

	long double ToLongDouble(Type type) const
	{
		return convesion_table[this->type][type](t);
	}

private:
	long double t;
	Type type;
	static std::map<Type, std::map<Type, std::function<long double(long double)>>> convesion_table;
};

std::map<Temperature::Type, std::map<Temperature::Type, std::function<long double(long double)>>> Temperature::convesion_table{
	{ Temperature::Type::Celsius, {
		{ Temperature::Type::Celsius,		[](long double t) { return t; } },
		{ Temperature::Type::Fahrenheit,	[](long double t) { return t * 9.0 / 5.0 + 32.0; } },
		{ Temperature::Type::Kelvin,		[](long double t) { return t + 273.15; } }
	} },
	{ Temperature::Type::Fahrenheit, {
		{ Temperature::Type::Celsius,		[](long double t) { return (t - 32.0) * 5.0 / 9.0; } },
		{ Temperature::Type::Fahrenheit,	[](long double t) { return t;  } },
		{ Temperature::Type::Kelvin,		[](long double t) { return (t - 32.0) * 5.0 / 9.0 + 273.15; } }
	} },
	{ Temperature::Type::Kelvin, {
		{ Temperature::Type::Celsius,		[](long double t) { return t - 273.15; } },
		{ Temperature::Type::Fahrenheit,	[](long double t) { return (t - 273.15) * 9.0 / 5.0 + 32.0;  } },
		{ Temperature::Type::Kelvin,		[](long double t) { return t; } }
	} }
};

Temperature operator""_deg(long double t)
{
	return Temperature(t, Temperature::Type::Celsius);
}

Temperature operator""_F(long double t)
{
	return Temperature(t, Temperature::Type::Fahrenheit);
}

Temperature operator""_K(long double t)
{
	return Temperature(t, Temperature::Type::Kelvin);
}

int main()
{
	{
		auto t1{ 36.5_deg };

		auto tf = Temperature(t1.ToLongDouble(Temperature::Type::Fahrenheit), Temperature::Type::Fahrenheit);
		auto tk = Temperature(tf.ToLongDouble(Temperature::Type::Kelvin), Temperature::Type::Kelvin);
		auto tc = Temperature(tk.ToLongDouble(Temperature::Type::Celsius), Temperature::Type::Celsius);

		assert(t1 == tf);
		assert(t1 == tk);
		assert(t1 == tc);
	}

	{
		auto tc{ 36.5_deg };
		auto tf{ 97.7_F };
		auto tk{ 309.65_K };

		assert(tc == tf);
		assert(tf == tk);
		assert(tk == tc);
		assert(tk > 309.0_K);
	}

	return 0;
}