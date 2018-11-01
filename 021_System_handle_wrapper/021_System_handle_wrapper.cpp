// 021_System_handle_wrapper.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <Windows.h>

template <typename Traits>
class unique_handle
{
	using pointer = typename Traits::pointer;
	pointer m_value;
public:
	unique_handle(unique_handle const &) = delete;
	unique_handle& operator=(unique_handle const &) = delete;

	explicit unique_handle(pointer value = Traits::invalid()) noexcept
		:m_value{ value }
	{}

	unique_handle(unique_handle && other) noexcept
		: m_value{ other.release() }
	{}

	unique_handle& operator=(unique_handle && other) noexcept
	{
		if (this != &other)
			reset(other.release());
		return *this;
	}

	~unique_handle() noexcept
	{
		Traits::close(m_value);
	}

	explicit operator bool() const noexcept
	{
		return m_value != Traits::invalid();
	}

	pointer get() const noexcept { return m_value; }

	pointer release() noexcept
	{
		auto value = m_value;
		m_value = Traits::invalid();
		return value;
	}

	bool reset(pointer value = Traits::invalid()) noexcept
	{
		if (m_value != value)
		{
			Traits::close(m_value);
			m_value = value;
		}
		return static_cast<bool>(*this);
	}

	void swap(unique_handle<Traits> & other) noexcept
	{
		std::swap(m_value, other.m_value);
	}
};

template <typename Traits>
void swap(unique_handle<Traits> & left, unique_handle<Traits> & right) noexcept
{
	left.swap(right);
}

template <typename Traits>
bool operator==(unique_handle<Traits> const & left,
	unique_handle<Traits> const & right) noexcept
{
	return left.get() == right.get();
}

template <typename Traits>
bool operator!=(unique_handle<Traits> const & left,
	unique_handle<Traits> const & right) noexcept
{
	return left.get() != right.get();
}

struct null_handle_traits
{
	using pointer = HANDLE;
	static pointer invalid() noexcept { return nullptr; }
	static void close(pointer value) noexcept
	{
		CloseHandle(value);
	}
};

struct invalid_handle_traits
{
	using pointer = HANDLE;
	static pointer invalid() noexcept { return INVALID_HANDLE_VALUE; }
	static void close(pointer value) noexcept
	{
		CloseHandle(value);
	}
};

using null_handle = unique_handle<null_handle_traits>;
using invalid_handle = unique_handle<invalid_handle_traits>;

void good_handle_example()
{
	bool condition1 = false;
	bool condition2 = true;

	invalid_handle handle{
	   CreateFile(L"sample.txt",
				  GENERIC_READ,
				  FILE_SHARE_READ,
				  nullptr,
				  OPEN_EXISTING,
				  FILE_ATTRIBUTE_NORMAL,
				  nullptr) };

	if (!handle) return;

	if (condition1) return;

	std::vector<char> buffer(1024);
	unsigned long bytesRead = 0;
	ReadFile(handle.get(),
		buffer.data(),
		static_cast<DWORD>(buffer.size()),
		&bytesRead,
		nullptr);

	if (condition2) return;

	//function_that_throws();
}

int main()
{
	good_handle_example();

	return 0;
}