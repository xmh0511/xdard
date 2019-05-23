#include <iostream>
#include "serialize.hpp"
struct info
{
	std::string address = "China";
	int number = 1100;
};
REFLECTION(info, address, number)
struct Test
{
	int age = 10;
	std::string name = "hello";
	info info_;
};
REFLECTION(Test,age,name, info_)
int main()
{
	nlohmann::json root;
	Test t;
	xmh::serializer::to_json(t, root);
	std::cout << root.dump() << std::endl;
	root["age"] = 20;
	Test t2;
	xmh::serializer::to_object(t2, root);
	std::cin.get();
	return 0;
}