// 046_Circular_buffer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::circular_buffer 이용

#include "pch.h"
#include <boost/circular_buffer.hpp>
#include <iostream>

int main()
{
	boost::circular_buffer<int> cbuf(5); // {0, 0, 0, 0, 0} -> {}

	cbuf.push_back(1);                 // {1, 0, 0, 0, 0} -> {1}
	cbuf.push_back(2);                 // {1, 2, 0, 0, 0} -> {1, 2}
	cbuf.push_back(3);                 // {1, 2, 3, 0, 0} -> {1, 2, 3}

	auto item = cbuf.front();

	cbuf.pop_front();					 // {1, 2, 3, 0, 0} -> {2, 3}
	cbuf.push_back(4);                 // {1, 2, 3, 4, 0} -> {2, 3, 4}
	cbuf.push_back(5);                 // {1, 2, 3, 4, 5} -> {2, 3, 4, 5}
	cbuf.push_back(6);                 // {6, 2, 3, 4, 5} -> {2, 3, 4, 5, 6}

	cbuf.push_back(7);                 // {6, 7, 3, 4, 5} -> {3, 4, 5, 6, 7}
	cbuf.push_back(8);                 // {6, 7, 8, 4, 5} -> {4, 5, 6, 7, 8}

	item = cbuf.front();
	cbuf.pop_front();            // {6, 7, 8, 4, 5} -> {5, 6, 7, 8}
	item = cbuf.front();
	cbuf.pop_front();            // {6, 7, 8, 4, 5} -> {6, 7, 8}
	item = cbuf.front();
	cbuf.pop_front();            // {6, 7, 8, 4, 5} -> {7, 8}

	item = cbuf.front();
	cbuf.pop_front();            // {6, 7, 8, 4, 5} -> {8}
	item = cbuf.front();
	cbuf.pop_front();            // {6, 7, 8, 4, 5} -> {}

	cbuf.push_back(9);                 // {6, 7, 8, 9, 5} -> {9}

	std::copy(std::cbegin(cbuf), std::cend(cbuf), std::ostream_iterator<int>(std::cout, ", "));
	std::cout << std::endl;

	return 0;
}