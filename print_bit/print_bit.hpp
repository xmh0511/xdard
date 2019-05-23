#pragma once
#include <iostream>
#include <vector>
#include <string>
namespace xmh {
	class print_bit {
		template<typename T>
		friend std::string bit(T&& t);
	private:
		static bool is_big()
		{
			int c = 1;
			const char* ptr = (char*)&c;
			if (*ptr) {
				return false;
			}
			return true;
		}
		static std::string printbit(const char* src, std::size_t n)
		{
			std::vector<char> buff;
			if (is_big()) {  //大端
				for (std::size_t i = 0; i < n; i++) {
					buff.push_back(*src);
					++src;
				}
			}
			else { //小端
				src = src + n - 1;
				for (std::size_t i = 0; i < n; i++) {
					buff.push_back(*src);
					src--;
				}
			}

			std::string bit_show;
			for (auto& iter : buff)
			{
				char c = 1;
				for (int count = 1; count <= 8; count++) {
					char tmp = iter >> (8 - count);
					tmp = (tmp & c) + '0';
					bit_show.append({ tmp });
				}
				bit_show.append(" ");
			}
			return bit_show;
		}
		template<typename T>
		static std::string printbit(std::vector<T>&& v)
		{
			return printbit(v);
		}

		template<typename T>
		static std::string printbit(std::vector<T>& v)
		{
			std::string result;
			for (auto& iter : v)
			{
				auto r = printbit((char*)&iter, sizeof(T));
				result.append(r);
				result.append(" ");
			}
			return result;
		}
		 template<typename T>
		 static std::string printbit(T&& t)  //内置类型 二进制显示
		 {
			auto r = printbit((char*)&t, sizeof(std::remove_reference_t<T>));
			return r;
		 }
	};
	template<typename T>
	std::string bit(T&& t)
	{
		return print_bit::printbit(std::forward<T>(t));
	}
}