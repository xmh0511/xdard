#include <iostream>
#include "currying.hpp"
using namespace xmh;
void show(int a, char b)
{
	std::cout << a << " " << b << std::endl;
}

class Object
{
public:
	void show(int a, char b)
	{
		std::cout << a << " member " << b << std::endl;
	}
};
int main()
{
	auto lambda = [](int a, char b) {
		std::cout << a << " == " << b << std::endl;
	};
	auto f0 = to_currying(lambda);
	f0(10)('l');

	auto f1 = to_currying(&show);
	f0(10)('r');

	Object obj;
	auto f2 = to_currying(&Object::show, obj);
	auto& b = f2(10);
	b('c');

	auto f3 = to_currying(&Object::show, &obj);
	f3(30)('c');

	std::cin.get();
	return 0;
}