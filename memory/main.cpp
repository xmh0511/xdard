#include <iostream>
#include "memory.hpp"
class Base
{
public:
	int base_v = 10;
};
class Test:public Base
{
public:
	Test() = default;
	Test(int a)
	{
		std::cout << "a = " << a << "construct " << std::endl;
		base_v = a;
	}
	~Test()
	{
		std::cout << "destory" << std::endl;
	}
	int a = 1;
};
int main()
{
	auto ptr = xmh::make_unique<Test>();
	xmh::unique_ptr<Test> ptr2;
	ptr2 = std::move(ptr);
	std::cout << ptr.get() << std::endl;
	std::cout << ptr2->a << std::endl;
	std::cout << (*ptr2).a << std::endl;

	xmh::shared_ptr<Base> ptr0 = xmh::make_shared<Test>(1024);
	xmh::shared_ptr<Test> ptr1;
	{
		xmh::shared_ptr<Test> ptr(new Test());
		ptr1 = ptr;
	}
	std::cout << ptr0->base_v << std::endl;
	std::cin.get();
	return 0;
}