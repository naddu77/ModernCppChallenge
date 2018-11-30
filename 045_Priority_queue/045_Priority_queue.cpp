// 045_Priority_queue.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - std::priority_queue 이용

#include "pch.h"
#include <iostream>
#include <queue>
#include <cassert>

int main()
{
	std::priority_queue<int> q;
	for (int i : {1, 5, 3, 1, 13, 21, 8})
	{
		q.push(i);
	}

	assert(!q.empty());
	assert(q.size() == 7);

	while (!q.empty())
	{
		std::cout << q.top() << ' ';
		q.pop();
	}

	return 0;
}