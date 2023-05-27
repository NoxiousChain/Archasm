#pragma once

#include "../../../Common.hpp"

#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

struct ThreadPool {
	vector<thread> workerThreads;
	queue<function<void()>> tasks;
	mutex queueMutex;
	condition_variable cv, cvWait;
	bool stop;
	int numActiveTasks = 0;

	ThreadPool(size_t numThreads);
	~ThreadPool();

	// Adds a function to the queue
	template<class F>
	void enqueue(F&& f) {
		{
			lock_guard<mutex> lock(queueMutex);
			tasks.emplace(forward<F>(f));
			numActiveTasks++;
		}
		cv.notify_one();
	}

	void wait();
};