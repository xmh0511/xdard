#include <iostream>
#include "function.hpp"
#include <functional>
void func(int& a) {
	std::cout << "func :" << a << std::endl;
}
class Test {
public:
	void call(int a) {
		std::cout << "Test: " << a << std::endl;
	}
};
int main() {
	xmh::function<void(int)> f = [](int v) {
		std::cout << v << std::endl;
	};
	f(10);
	xmh::function<void(int&)> f1 = &func;
	int a = 30;
	f1(a);
	f(a);
	Test t;
	xmh::function<void(int)> f2 = std::bind(&Test::call, t, std::placeholders::_1);
	f2(1024);
	std::cin.get();
	return 0;
}