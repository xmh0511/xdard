#include <iostream>
#include "tuple.hpp"
#define println(x)  std::cout<<x<<std::endl;

int main()
{
	xmh::tuple<int,double> tp(10,20.13);
	println(typeid(xmh::get_tuple_element_t<1, xmh::tuple<int, double>>).name());
	println(xmh::get<0>(tp));
	xmh::get<0>(tp) = 1024;
	println(xmh::get<0>(tp));
	println(xmh::get<double>(tp));

	println("------------------------");

	auto ntp = xmh::tuple_cat(tp, xmh::tuple<char>('c'));
	println(typeid(ntp).name());

	std::cin.get();
	return 0;
}