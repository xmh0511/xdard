#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
template<typename T>
class simple_pool
{
	using element_type = std::shared_ptr<T>;
public:
	simple_pool(std::size_t size):pool_(size){
		pool_.reserve(size);
	}
public:
	template<typename Function>
	void init_pool(Function&& function) {
		for (auto& iter : pool_) {
			function(iter);
		}
	}
public:
	element_type takeout() {
		std::unique_lock<std::mutex> lock(mutex_);
		cdva.wait(lock, [this]() {
			return !pool_.empty();
		});
		auto data = pool_.back();
		pool_.pop_back();
		return data;
	}
	void revert(std::weak_ptr<T> eleref) {
		std::unique_lock<std::mutex> lock(mutex_);
		pool_.push_back(eleref.lock());
		lock.unlock();
		cdva.notify_one();
	}
private:
	std::vector<element_type> pool_;
	std::mutex mutex_;
	std::condition_variable cdva;
};