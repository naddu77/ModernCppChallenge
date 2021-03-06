// 037_Finding_files_in_a_directory_that.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// Pred를 Patial Application을 이용
// 출력할 때 for문 사용 안함

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <functional>

namespace fs = std::experimental::filesystem;

std::vector<fs::directory_entry> FIndFiles(fs::path const & path, std::string_view regex)
{
	std::vector<fs::directory_entry> result;
	std::regex rx(regex.data());

	auto Pred = [](fs::directory_entry const & entry, std::regex const& rx) {
		return fs::is_regular_file(entry.path()) &&
			std::regex_match(entry.path().filename().string(), rx);
	};

	std::copy_if(
		fs::recursive_directory_iterator(path),
		fs::recursive_directory_iterator(),
		std::back_inserter(result),
		std::bind(Pred, std::placeholders::_1, std::cref(rx))
	);

	return result;
}

int main()
{
	auto dir = fs::temp_directory_path();
	auto pattern = R"(wct[0-9a-zA-Z]{3}\.tmp)";
	auto result = FIndFiles(dir, pattern);
	std::vector<std::string> str_result;

	std::transform(std::cbegin(result), std::cend(result), std::back_inserter(str_result), [](auto const& e) {
		return e.path().string();
	});
	std::copy(std::cbegin(str_result), std::cend(str_result), std::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}
