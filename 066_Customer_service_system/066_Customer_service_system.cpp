// 066_Customer_service_system.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Todo: 코드 분석 해보자.

#include "pch.h"
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <random>
#include <array>
#include <string>
#include <string_view>

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

class ticketing_machine
{
public:
	ticketing_machine(int const start) :
		last_ticket(start), first_ticket(start)
	{}

	int next() { return last_ticket++; }
	int last() const { return last_ticket - 1; }
	void reset() { last_ticket = first_ticket; }
private:
	int first_ticket;
	int last_ticket;
};

class customer
{
public:
	customer(int const no) : number(no) {}

	int ticket_number() const noexcept { return number; }
private:
	int number;
	friend bool operator<(customer const & l, customer const & r);
};

bool operator<(customer const & l, customer const & r)
{
	return l.number > r.number;
}

int main()
{
	std::priority_queue<customer> customers;
	bool office_open = true;
	std::mutex mt;
	std::condition_variable cv;

	std::vector<std::thread> desks;
	for (int i = 1; i <= 3; ++i)
	{
		desks.emplace_back([i, &office_open, &mt, &cv, &customers]() {
			std::random_device rd;
			auto seed_data = std::array<int, std::mt19937::state_size> {};
			std::generate(std::begin(seed_data), std::end(seed_data),
				std::ref(rd));
			std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
			std::mt19937 eng(seq);
			std::uniform_int_distribution<> ud(2000, 3000);

			logger<MultiThreadPolicy>::instance().log("desk " + std::to_string(i) + " open");

			while (office_open || !customers.empty())
			{
				std::unique_lock<std::mutex> locker(mt);

				cv.wait_for(locker, std::chrono::seconds(1),
					[&customers]() {return !customers.empty(); });

				if (!customers.empty())
				{
					auto const c = customers.top();
					customers.pop();

					logger<MultiThreadPolicy>::instance().log(
						"[-] desk " + std::to_string(i) + " handling customer "
						+ std::to_string(c.ticket_number()));

					logger<MultiThreadPolicy>::instance().log(
						"[=] queue size: " + std::to_string(customers.size()));

					locker.unlock();
					cv.notify_one();

					std::this_thread::sleep_for(
						std::chrono::milliseconds(ud(eng)));

					logger<MultiThreadPolicy>::instance().log(
						"[ ] desk " + std::to_string(i) + " done with customer "
						+ std::to_string(c.ticket_number()));
				}
			}

			logger<MultiThreadPolicy>::instance().log("desk " + std::to_string(i) + " closed");
		});
	}

	std::thread store([&office_open, &customers, &mt, &cv]() {
		ticketing_machine tm(100);

		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data),
			std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 eng(seq);
		std::uniform_int_distribution<> ud(200, 500);

		for (int i = 1; i <= 25; ++i)
		{
			customer c(tm.next());
			customers.push(c);

			logger<MultiThreadPolicy>::instance().log("[+] new customer with ticket " +
				std::to_string(c.ticket_number()));
			logger<MultiThreadPolicy>::instance().log("[=] queue size: " +
				std::to_string(customers.size()));

			cv.notify_one();

			std::this_thread::sleep_for(std::chrono::milliseconds(ud(eng)));
		}

		office_open = false;
	});

	store.join();
	for (auto & desk : desks) 
		desk.join();

	return 0;
}