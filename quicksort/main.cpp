#include "quicksort.hpp"
#include <vector>
struct data {
	int value;
};
int main() {
	data arr[10] = { {0},{100},{20},{18},{96},{30},{5},{7},{50},{29} };
	//std::vector<int> vec;
	//vec.resize(1024);
	xmh::quick_sort(arr, [](auto& v) {
		return v.value;
	});
	//xmh::quick_sort(vec);
	for (auto i = 0; i < 10; i++) {
		std::cout << arr[i].value << std::endl;
	}
	std::cin.get();
}