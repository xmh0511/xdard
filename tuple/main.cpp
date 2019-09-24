#include <iostream>
#include "tuple.hpp"

int main() {
	int a = 10;
	int b = 1024;
	float c = 0.12f;
	xmh::tuple<int&, float> tp0 = { a ,c};
	xmh::tuple<int&, float> tp1 = {b};
	tp0 = tp1;
	std::cout << xmh::get<int&>(tp0) << std::endl;
	std::cout << xmh::get<1>(tp0) << std::endl;
	xmh::get<float>(tp0) = 2.1f;
	std::cout << xmh::get<1>(tp0) << std::endl;
	std::cout << "-------------------" << std::endl;
	xmh::tuple<char> tp2 = 'c';
	auto tp3 = xmh::tuple_cat(tp0, tp2);
	xmh::tuple<int&, float> const& vv = tp0;
	xmh::get<float>(vv);
	std::cout << typeid(tp3).name() << std::endl;
	std::cout << xmh::get<0>(tp3) << std::endl;
	std::cout << xmh::get<1>(tp3) << std::endl;
	std::cout << xmh::get<2>(tp3) << std::endl;
	xmh::tuple<int const&> tp4 = a;
	auto tp5 = xmh::tuple_cat(tp3, tp4);
	std::cin.get();
}