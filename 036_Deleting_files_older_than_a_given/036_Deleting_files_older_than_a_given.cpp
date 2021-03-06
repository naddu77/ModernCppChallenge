// 036_Deleting_files_older_than_a_given.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - For 문을 없애고 싶다.

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <cctype>

namespace fs = std::experimental::filesystem;
namespace ch = std::chrono;

template <typename Duration>
bool is_older_than(fs::path const & path, Duration const duration)
{
	auto ftimeduration = fs::last_write_time(path).time_since_epoch();
	auto nowduration = (ch::system_clock::now() - duration)
		.time_since_epoch();
	return ch::duration_cast<Duration>(nowduration - ftimeduration)
		.count() > 0;
}

template <typename Duration>
void remove_files_older_than(fs::path const & path,
	Duration const duration)
{
	try
	{
		if (fs::exists(path))
			if (is_older_than(path, duration))
				fs::remove(path);
			else if (fs::is_directory(path))
				for (auto const & entry : fs::directory_iterator(path))
					remove_files_older_than(entry.path(), duration);
	}
	catch (std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

int main()
{
	using namespace std::chrono_literals;

#ifdef _WIN32
	auto path = R"(..\Test\)";
#else
	auto path = R"(../Test/)";
#endif

	remove_files_older_than(path, 1h + 20min);

	return 0;
}