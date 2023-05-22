#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numThreads)
	: stop{ false }
{
	for (size_t i = 0; i < numThreads; i++) {
		workerThreads.emplace_back([this] {
			while (true) {
				function<void()> task;
				{
					unique_lock<mutex> lock(this->queueMutex);
					this->cv.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
					if (this->stop && this->tasks.empty()) return;
					task = std::move(this->tasks.front());
					this->tasks.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	{
		unique_lock<mutex> lock(queueMutex);
		stop = true;
	}
	cv.notify_all();
	for (thread& worker : workerThreads) {
		worker.join();
	}
}
