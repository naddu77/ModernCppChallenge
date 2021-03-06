// 038_Temporary_log_files.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - UUID 라이브러리는 다양해서 정책 클래스로 분리(OCP, CRTP)
// - static_assert를 이용해서 이상한 정책 클래스면 컴파일 에러나게 변경

#include "pch.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <uuid.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

template <typename Derived>
class UUIDS
{
public:
	static std::string GetUUID(void)
	{
		return Derived::GetUUID();
	}
};

class BoostUUIDS
	: public UUIDS<BoostUUIDS>
{
public:
	static std::string GetUUID(void)
	{
		return boost::uuids::to_string(boost::uuids::random_generator{}());
	}
};

class StdUUIDS
	: public UUIDS<StdUUIDS>
{
public:
	static std::string GetUUID(void)
	{
		return uuids::to_string(uuids::uuid_random_generator{}());
	}
};

class IsNotUUID_Strategy{};

template <typename UUIDS_Strategy>
class Logger
{
	static_assert(std::is_base_of_v<UUIDS<UUIDS_Strategy>, UUIDS_Strategy>, "is not UUID Strategy");

public:
	Logger()
	{
		auto name = UUIDS_Strategy::GetUUID();

		logpath = fs::temp_directory_path() / (name + ".tmp");
		logfile.open(logpath.c_str(), std::ios::out | std::ios::trunc);
	}

	~Logger() noexcept
	{
		try 
		{
			if (logfile.is_open()) 
				logfile.close();

			if (!logpath.empty()) 
				fs::remove(logpath);
		}
		catch (...) {}
	}

	void Persist(fs::path const & path)
	{
		logfile.close();
		fs::rename(logpath, path);
		logpath.clear();
	}

	Logger& operator<<(std::string_view message)
	{
		logfile << message.data() << '\n';
		return *this;
	}

private:
	fs::path logpath;
	std::ofstream logfile;
};

int main()
{
	Logger<BoostUUIDS> log;
	//Logger<StdUUIDS> log;
	//Logger<IsNotUUID_Strategy> ok;

	try
	{
		log << "this is a line" << "and this is another one";

		throw std::runtime_error("error");
	}
	catch (...)
	{
		log.Persist(R"(lastlog.txt)");
	}

	return 0;
}
