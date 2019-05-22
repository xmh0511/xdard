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
	{
		xmh::xarray<Test[2]> temp;
	}
	xmh::xarray<int[2][3][4]> arr;
	arr[0][1][1] = 10;
	auto c = std::move(arr);
	std::cout << c[0][1][1] << std::endl;
	std::cin.get();
	return 0;
}