#include <iostream>
#include "memory.hpp"
class Test
{
public:
	Test() = default;
	Test(int a)
	{
		std::cout << "a = " << a << "construct " << std::endl;
	}
	~Test()
	{
		std::cout << "destory" << std::endl;
	}
	int a = 1024;
};
int main()
{
	//auto ptr = xmh::make_unique<Test>();
	//xmh::unique_ptr<Test> ptr2;
	//ptr2 = std::move(ptr);
	//std::cout << ptr.get() << std::endl;
	//std::cout << ptr2->a << std::endl;
	//std::cout << (*ptr2).a << std::endl;
	xmh::shared_ptr<Test> ptr0 = xmh::make_shared<Test>(1024);
	{
		xmh::shared_ptr<Test> ptr(new Test());
		ptr0 = ptr;
	}
	std::cin.get();
	return 0;
}