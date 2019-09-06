#pragma once
#include <iostream>
namespace xmh {
	template<typename...Args>
	struct type_contain {

	};

	template<bool Result, typename T, typename U>
	struct comparetor {
		using max = U;
		using min = T;
	};

	template<typename T, typename U>
	struct comparetor<true, T, U> {
		using max = T;
		using min = U;
	};

	template<typename T, typename U>
	using max_type = typename comparetor<(sizeof(T) > sizeof(U)), T, U>::max;

	template<typename T, typename U>
	using min_type = typename comparetor<(sizeof(T) > sizeof(U)), T, U>::min;

	template<std::size_t Start, std::size_t End, typename Contain, typename...Args>
	struct type_order_c {

	};

	template<std::size_t Start, std::size_t End, typename...Contains, typename T, typename U, typename...Args>
	struct type_order_c<Start, End, type_contain<Contains...>, T, U, Args...> {
		using type = typename type_order_c<Start, End, type_contain<Contains..., min_type<T, U>>, max_type<T, U>, Args... >::type;
	};

	template<std::size_t Start, std::size_t End, typename...Contains, typename T>
	struct type_order_c<Start, End, type_contain<Contains...>, T> {
		using type = typename type_order_c<Start + 1, End, type_contain<>, Contains..., T>::type;
	};

	template<std::size_t End, typename...Contains, typename T>
	struct type_order_c<End, End, type_contain<Contains...>, T> {
		using type = type_contain<Contains..., T>;
	};

	template<typename...Args>
	struct type_order {
		using type = typename type_order_c<0, (sizeof...(Args) * sizeof...(Args)), type_contain<>, Args...>::type;
	};
}