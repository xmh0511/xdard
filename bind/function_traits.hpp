#pragma once
#include "tuple.hpp"
namespace xmh
{
	template<typename T, typename U = void>
	struct function_traits
	{

	};

	template<typename Ret,typename...Args>
	struct function_traits<Ret(*)(Args...), void>
	{
		using ret = Ret;
		using params_type_tuple = xmh::tuple<Args...>;
		static constexpr int args_size = sizeof...(Args);
		static constexpr bool is_member_function = false;
	};

	template<typename Ret,typename Class,typename...Args>
	struct function_traits<Ret(Class::*)(Args...), void>
	{
		using ret = Ret;
		using params_type_tuple = xmh::tuple<Args...>;
		static constexpr int args_size = sizeof...(Args);
		static constexpr bool is_member_function = true;
	};

	template<typename Ret, typename Class, typename...Args>
	struct function_traits<Ret(Class::*)(Args...) const, void>
	{
		using ret = Ret;
		using params_type_tuple = xmh::tuple<Args...>;
		static constexpr int args_size = sizeof...(Args);
		static constexpr bool is_member_function = false;
	};

	template<typename Function>
	struct function_traits<Function, std::void_t<decltype(&Function::operator())>>:function_traits<decltype(&Function::operator()),void>
	{

	};
}

