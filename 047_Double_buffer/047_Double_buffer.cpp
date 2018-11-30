// 047_Double_buffer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 저자 코드 그대로

#include "pch.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iterator>

template <typename T>
class DoubleBuffer
{
	typedef T           value_type;
	typedef T&          reference;
	typedef T const &   const_reference;
	typedef T*          pointer;
public:
	explicit DoubleBuffer(size_t const size)
		: rdbuf(size), wrbuf(size)
	{
	
	}

	size_t Size() const noexcept
	{ 
		return rdbuf.size(); 
	}

	void Write(T const * const ptr, size_t const size)
	{
		std::unique_lock<std::mutex> lock(mt);

		std::copy(ptr, ptr + std::min(size, wrbuf.size()), std::begin(wrbuf));
		wrbuf.swap(rdbuf);
	}


	template <class Output>
	void Read(Output it) const
	{
		std::unique_lock<std::mutex> lock(mt);

		std::copy(std::cbegin(rdbuf), std::cend(rdbuf), it);
	}

	pointer Data() const
	{
		std::unique_lock<std::mutex> lock(mt);

		return rdbuf.data();
	}

	reference operator[](size_t const pos)
	{
		std::unique_lock<std::mutex> lock(mt);

		return rdbuf[pos];
	}

	const_reference operator[](size_t const pos) const
	{
		std::unique_lock<std::mutex> lock(mt);

		return rdbuf[pos];
	}

	void Swap(DoubleBuffer other)
	{
		std::swap(rdbuf, other.rdbuf);
		std::swap(wrbuf, other.wrbuf);
	}

private:
	std::vector<T>     rdbuf;
	std::vector<T>     wrbuf;
	mutable std::mutex mt;
};

template <typename T>
void PrintBuffer(DoubleBuffer<T> const & buf)
{
	buf.Read(std::ostream_iterator<T>(std::cout, " "));
	std::cout << std::endl;
}

int main()
{
	DoubleBuffer<int> buf(10);

	std::thread t([&buf]() {
		for (int i = 1; i < 1000; i += 10)
		{
			int data[] = { i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7, i + 8, i + 9 };
			
			buf.Write(data, 10);

			using namespace std::chrono_literals;

			std::this_thread::sleep_for(100ms);
		}
	});

	auto start = std::chrono::system_clock::now();

	do
	{
		PrintBuffer(buf);

		using namespace std::chrono_literals;

		std::this_thread::sleep_for(150ms);
	} while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 12);

	t.join();

	return 0;
}