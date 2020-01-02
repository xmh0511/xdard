#include <iostream>
#include <thread>
#include "atomic.hpp"

int main() {
	xmh::atomic<int> atomic = { 25 };
	std::thread t0([&atomic]() {
		for (auto i = 0; i < 5; i++) {
			atomic--;
		}
	});
	std::thread t1([&atomic]() {
		for (auto i = 0; i < 5; i++) {
			atomic--;
		}
		});
	std::thread t2([&atomic]() {
		for (auto i = 0; i < 15; i++) {
			atomic--;
		}
	});
	t0.join();
	t1.join();
	t2.join();
	atomic--;
	xmh::atomic<bool> atomic_bool = { false };
	std::thread t3([&atomic_bool]() {
		for (auto i = 0; i < 6; i++) {
			atomic_bool = false;
		}
	});
	std::thread t4([&atomic_bool]() {
		for (auto i = 0; i < 5; i++) {
			atomic_bool = true;
		}
	});
	t3.join();
	t4.join();
	std::cout << atomic << std::endl;
	std::cout << atomic_bool << std::endl;
	std::getchar();
}