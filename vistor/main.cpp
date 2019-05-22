#include <iostream>
#include "vistor.hpp"
class Data
{
public:
	template<typename Vistor>
	void vistor(Vistor& vister)
	{
		vister & reg_field(people_name);
		vister & reg_field(age);
	}
private:
	std::string people_name ="hello,world";
	int age = 30;
};
int main()
{
	auto f = [](auto& n,auto& v) {
		std::cout <<n<<" : "<< v << std::endl;
	};
	Data t;
	auto v = xmh::make_vistor(t,f);
	v.each();
	v.find("age");
	std::cin.get();
	return 0;
}