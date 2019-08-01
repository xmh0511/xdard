#include <iostream>
#include "oberver.hpp"
using namespace xmh;
int main()
{
	Observer t;
	t.subscribe<Ref<data>>("click", [](auto data) {
		std::cout << data.get().id << std::endl;
		data.get().id = 30;
	});
	t.subscribe<int>("click", [](auto data) {
		std::cout << data << std::endl;
	});
	data d;
	t.dispatch("click", ref(d));
	t.dispatch("click", 50);
	std::cin.get();
}