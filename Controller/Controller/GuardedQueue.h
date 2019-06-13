#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include "MicroController.h"
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
			MicroController* a = new MicroController("/dev/ttyACM0");
			return Command(a, "Wait");
		}
		else if(m_queque.size() > 50) {
			std::cout << "CommanndQueueToLarge!" << std::endl;
		}
		T result = m_queque.front();
		m_queque.pop();
		return result;
	}
	int GetSize() {
	   return m_queque.size();
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
