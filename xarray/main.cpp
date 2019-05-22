#include <iostream>
#include "xarray.hpp"
class Test
{
public:
	~Test() {
		std::cout << "destory" << std::endl;
	}
};
int main()
{
	//std::cout << typeid(typename contact_type<0, 3, reverse_content<content<1, 2, 3>, content<>>::type, int, int>::type).name() << std::endl;
	//std::cout << typeid(reverse_content<content<1, 2, 3>, content<>>::type).name() << std::endl;
	{
		xmh::xarray<Test[2]> temp;
	}
	xmh::xarray<int[2][3][4]> arr;
	arr[0][1][1] = 10;
	auto c = std::move(arr);
	std::cout << c[0][1][1] << std::endl;
	/////////////////
	xmh::xarray<int, 2,6> arr0;
	arr0[0][0] = 1;
	std::cout << arr0[0][0] << std::endl;
	std::cin.get();
	return 0;
}