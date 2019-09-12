#include "utility.hpp"
class Data {
public:
	Data() = default;
	Data(Data const&) {
		std::cout << "copy" << std::endl;
	}
	//Data& operator=(Data const&) {
	//	return *this;
	//}
	Data(Data&&) = delete;
	//Data(Data&&) noexcept {
	//	std::cout << "rRefrence" << std::endl;
	//}
};



int main() {
	std::cout << xmh::has_move_constructor_v<Data> << std::endl;
	std::cin.get();
}