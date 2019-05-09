#include<mutex>
#include<condition_variable>

std::mutex mtx;
std::condition_variable cv;

void push(T* elem) {
	std::unique_lock<std::mutex> lck(mtx);
	if (elem != nullptr) {
		q.push(elem);
		cv.notify_one();
	}
}

T* next() {
	T* elem = nullptr;

	std::unique_lock<std::mutex> lck(mtx);
	cv.wait(lck, !q.empty());

	if (!q.empty()) {
		elem = q.front();
		q.pop();
	}

	return elem;
}