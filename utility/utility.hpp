#pragma once
#include <iostream>
namespace xmh {
	template<typename T, typename U = void>
	struct has_copy_constructor {
		constexpr static bool value = false;
	};

	template<typename T>
	struct has_copy_constructor<T, std::void_t<decltype(T(std::declval<T const&>()))>> {
		constexpr static bool value = true;
	};

	template<typename T, typename U = void>
	struct accept_rvreference {
		constexpr static bool value = false;
	};

	template<typename T>
	struct accept_rvreference<T, std::void_t<decltype(T(std::move(std::declval<T>())))>> {
		constexpr static bool value = true;
	};

	template<typename T, typename U = void>
	struct has_move_constructor {
		constexpr static bool value = false;
	};

	template<typename T>
	struct has_move_constructor<T, std::enable_if_t<accept_rvreference<T>::value>> {
		constexpr static bool value = true;
	};

	template<typename T>
	constexpr bool has_copy_constructor_v = has_copy_constructor<T>::value;

	template<typename T>
	constexpr bool has_move_constructor_v = has_move_constructor<T>::value;
}