#include "pch.h"
#include <iostream>
#include <numeric>

int main()
{
	int a;
	int b;

	std::cin >> a >> b;
	std::cout << "Greatest common divisor=" << std::gcd(a, b) << std::endl;

	return 0;
}
