// 035_Computing_the_size_of_a_directory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - No comment

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <numeric>
#include <locale>

namespace fs = std::filesystem;

int main()
{
	auto GetFolderSize = [](auto const& path) {
		return std::reduce(fs::recursive_directory_iterator("."),
			fs::recursive_directory_iterator(), decltype(fs::file_size(std::declval<fs::path>())){},
			[](auto const acc, auto const& e) {
				return acc + fs::file_size(e);
			}
		);
	};

	std::cout.imbue(std::locale("kor"));
	std::cout << "Folder Size: " << GetFolderSize(".") << " bytes" << std::endl;
}