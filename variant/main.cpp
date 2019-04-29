#include <iostream>
#include "variant.hpp"
int main()
{
	xmh::variant<int, bool, char> b = 'c';
	std::cout << b.get<int>()<<std::endl;
	std::cout << b.type_index() <<"  "<<b.type_name() << std::endl;
	std::cin.get();
	return 0;
}