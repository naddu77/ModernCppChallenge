// 065_Thread-safe_logging_to_the_console.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - Template을 이용해서 Thread 정책을 정할 수 있게 함.

#include "pch.h"
#include <iostream>
#include <mutex>
#include <string_view>
#include <random>
#include <string>

class SingleThreadPolicy
{
public:
	inline void Lock(void)
	{

	}

	inline void Unlock(void)
	{

	}
};

class MultiThreadPolicy
{
public:
	inline void Lock(void)
	{
		m.lock();
	}

	inline void Unlock(void)
	{
		m.unlock();
	}

private:
	std::mutex m;
};

template <typename ThreadPolicy>
class logger
{
protected:
	logger() {}
public:
	static logger& instance()
	{
		static logger lg;

		return lg;
	}

	logger(logger const &) = delete;
	logger& operator=(logger const &) = delete;

	void log(std::string_view message)
	{
		thread_policy.Lock();
		std::cout << "LOG: " << message << std::endl;
		thread_policy.Unlock();
	}

private:
	ThreadPolicy thread_policy;
};

int main()
{
	std::vector<std::thread> modules;

	for (int id = 1; id <= 5; ++id)
	{
		modules.emplace_back([id]() {
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<> ud(100, 1000);

			logger<MultiThreadPolicy>::instance().log(
				"module " + std::to_string(id) + " started");

			std::this_thread::sleep_for(std::chrono::milliseconds(ud(mt)));

			logger<MultiThreadPolicy>::instance().log(
				"module " + std::to_string(id) + " finished");
		});
	}

	for (auto & m : modules)
		m.join();

	return 0;
}