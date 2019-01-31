#include <iostream>
#include "bind.hpp"

void show(int&& a,float& b)
{
	std::cout << a << "   " << b << std::endl;
	a = 12;
	b = 20.03;
}

class Test
{
public:
	void show(int a)
	{
		std::cout <<"OK  "<< a << std::endl;
	}
};
int main()
{
	int a = 10;
	auto b = xmh::bind(&show, std::move(a), xmh::placehold<1>{});
	float d = 20;
	b(d);
	std::cout << d<<"   "<<a << std::endl;


	auto ff = xmh::bind([](char c) { std::cout << c << std::endl; }, xmh::placehold<1>{});
	ff('c');

	Test t;
	auto cf = xmh::bind(&Test::show, t, 30);
	cf();

	std::cin.get();
	return 0;
}