#include <iostream>
#include "xlog.hpp"
#include <thread>
int main() {
	xmh::XLog::init("./","my");
	std::thread t0 = std::thread([]() {
		int i = 10;
		while ((i--) > 0) {
			LOGINFO << "dc";
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
		}
	});

	std::thread t1 = std::thread([]() {
		int i = 10;
		while ((i--) > 0) {
			LOGWARN << "waring 000";
			std::this_thread::sleep_for(std::chrono::milliseconds(802));
		}
	});

	std::thread t2 = std::thread([]() {
		int i = 10;
		while ((i--) > 0) {
			LOGERR << "err 123";
			std::this_thread::sleep_for(std::chrono::milliseconds(900));
		}
	});

	t0.join();
	t1.join();
	t2.join();
	std::cin.get();
	return 0;
}