#pragma once
#include <windows.h>

class HandleGuard
{
public:
	explicit HandleGuard(HANDLE h = nullptr) :
		m_handle(h) { }

	~HandleGuard()
	{
		if (m_handle && m_handle != INVALID_HANDLE_VALUE)
			CloseHandle(m_handle);
	}

	HandleGuard(const HandleGuard&)			   = delete;
	HandleGuard& operator=(const HandleGuard&) = delete;

	HANDLE get() const
	{
		return m_handle;
	}

	void reset(HANDLE h = nullptr)
	{
		if (m_handle && m_handle != INVALID_HANDLE_VALUE)
			CloseHandle(m_handle);
		m_handle = h;
	}

	operator HANDLE() const
	{
		return m_handle;
	}

private:
	HANDLE m_handle = nullptr;
};
