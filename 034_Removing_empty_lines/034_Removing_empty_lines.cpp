// 034_Removing_empty_lines.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - for 문을 없애고 싶다.

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <filesystem>

int main()
{
	std::ifstream ifs("sample.txt");

	if (ifs.is_open())
	{
		std::ofstream ofs("output.txt");

		if (ofs.is_open())
		{
			for (std::string line; std::getline(ifs, line);)
				if (!std::empty(line) && !std::any_of(std::cbegin(line), std::cend(line), [](unsigned char const e) {
					return std::isspace(e);
				}))
					ofs << line << std::endl;

			std::filesystem::remove("sample.txt");
			std::filesystem::rename("output.txt", "sample.txt");
		}
		else
			std::cerr << "파일 열기 실패!" << std::endl;
	}
	else
		std::cerr << "파일 열기 실패!" << std::endl;

	return 0;
}