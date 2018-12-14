// 070_Approval_system.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Chaining을 간단하게 구현

#include "pch.h"
#include <iostream>
#include <string>
#include <string_view>

struct Expense
{
	double amount;
	std::string description;

	Expense(double const amount, std::string_view desc) :
		amount(amount), description(desc)
	{
	}
};

class Employee
{
public:
	Employee(std::string_view name, double const appoval_limit)
		: name(name), appoval_limit(appoval_limit)
	{

	}

	Employee& operator()(Employee& other)
	{
		direct_manager = &other;

		return other;
	}

	void approve(Expense const& e)
	{
		if (e.amount <= appoval_limit)
			std::cout << name << " approved expense '" << e.description
			<< "', cost=" << e.amount << std::endl;
		else if (direct_manager != nullptr)
			direct_manager->approve(e);
		else
			std::cout << "could not approve expense.\n";
	}

private:
	std::string const name;
	double const appoval_limit;
	Employee* direct_manager = nullptr;
};

int main()
{
	Employee john{ "john smith", 1'000 };
	Employee robert{ "robert booth", 10'000 };
	Employee david{ "david jones", 100'000 };
	Employee cecil{ "cecil williamson", std::numeric_limits<double>::max() };

	john (robert) (david) (cecil);

	john.approve(Expense{ 500, "magazins" });
	john.approve(Expense{ 5000, "hotel accomodation" });
	john.approve(Expense{ 50000, "conference costs" });
	john.approve(Expense{ 200000, "new lorry" });

	return 0;
}