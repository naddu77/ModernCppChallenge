#include "pch.h"
#include <iostream>
#include <numeric>

int main()
{
	int a;
	int b;

	std::cin >> a >> b;
	std::cout << "Least common divisor=" << std::lcm(a, b) << std::endl;

	return 0;
}