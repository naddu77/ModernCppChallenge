// 028_Longest_palindromic_substring.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// note: Manacher's Algorithm 이용(https://algospot.com/wiki/read/Manacher's_algorithm)

#include "pch.h"
#include <iostream>
#include <string>
#include <cassert>
#include <string_view>
#include <algorithm>
#include <vector>

std::string LongestPalindrome(std::string const& str)
{
	std::vector<std::size_t> pal(std::size(str), 0u);
	std::size_t c = 0u;
	std::size_t r = 0u;
	auto len = std::size(str);

	for (auto i = 0u; i < len; ++i)
	{
		auto mirr = 2 * c - i;

		if (i < r)
			pal[i] = std::min(r - i, pal[mirr]);

		while (i - pal[i] > 0 && i + pal[i] - 1 < len && str[i - pal[i] - 1] == str[i + pal[i] + 1])
			++pal[i];

		if (pal[i] + i > r)
		{
			r = pal[i] + i;
			c = i;
		}
	}

	auto radius = std::max_element(std::cbegin(pal), std::cend(pal));
	auto start_pos = radius - std::cbegin(pal) - *radius;

	return str.substr(start_pos, *radius * 2 + 1);
}

int main()
{
	assert(LongestPalindrome("sahararahnide") == "hararah");
	assert(LongestPalindrome("level") == "level");
	assert(LongestPalindrome("s") == "s");

	return 0;
}

//int palindrome() 
//{
//	len = strlen(str);
//	int c = 0, r = 0;//c는 센터, r은 센터로부터 반경을 더한 값(앞으로 구할 인덱스가 r에 포함되어 있는지 판단하기 위해)
//	for (int i = 0; i < len; i++) {
//		int mirr = 2 * c - i;// mirr는 센터를 기준으로 나와 대칭인 곳의 값 위의 예시에서는 p[x]=2를 찾기 위함이다.
//
//		if (i < r)    //내가 반경안에 있으면 pal[mirr]값을 가져온다. 
//			pal[i] = min(r - i, pal[mirr]); // r-i는 오른쪽의 남은 범위인데 그 값만큼이 최대로 보장할 수 있는길이
//	   //이제 최소값을 구햇으니 그 밖의 범위만을 비교한다. 하나씩, 범위검사하고 비교하고 
//		while (i - pal[i] > 0 && i + pal[i] - 1 < len && str[i - pal[i] - 1] == str[i + pal[i] + 1])
//			pal[i]++;
//		//새로운 구간 갱신
//		if (pal[i] + i > r) {
//			r = pal[i] + i;
//			c = i;
//		}
//	}
//	int ret = 0;
//	for (int i = 0; i <= len; i++) {
//		int curLen = pal[i];
//		if (i % 2 == 0 && pal[i] % 2 == 1) 
//			curLen++;
//		ret = max(ret, curLen);
//	}
//	return ret;
//}