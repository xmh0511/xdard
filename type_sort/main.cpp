#include <iostream>
#include "ordertype.hpp"


int main() {
	std::cout << typeid(xmh::type_order<int, char, double, short>::type).name() << std::endl;
	std::cin.get();
}