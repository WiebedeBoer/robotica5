#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include "Arduino.h"
#include "Command.h"
template<typename T>
class GuardedQueue
{
public:
	void push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queque.push(value);
	}
	void pop(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queque.pop();
	}

	T GetFront()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queque.size() == 0) {
			Arduino a = Arduino();
			return Command(a, "Wait");
		}
		T result = m_queque.front();
		m_queque.pop();
		return result;
	}
	GuardedQueue();
	~GuardedQueue();

private:
	std::queue<T> m_queque;
	mutable std::mutex m_mutex;
};

template<typename T>
inline GuardedQueue<T>::GuardedQueue()
{
}

template<typename T>
inline GuardedQueue<T>::~GuardedQueue()
{
}
