#pragma once
#include <iostream>
namespace xmh {

	template<typename...Args>
	class tuple {};

	template<typename T, typename...Args>
	class tuple<T, Args...> :public tuple<Args...>
	{
	public:
		using element_type = T;
	public:
		constexpr tuple<T, Args...>() = default;

		constexpr tuple<T, Args...>(T v, Args...args) : tuple<Args...>(std::forward<Args>(args)...), value(std::forward<T>(v))
		{

		}
	public:
		T& get()
		{
			return value;
		}
	private:
		T value;
	};

	template<>
	class tuple<>
	{
	public:
		virtual ~tuple<>()
		{

		}
	};

	template<std::size_t N, typename...Args>
	struct get_tuple_type
	{
	};

	template<std::size_t N, typename T, typename...Args>
	struct get_tuple_type<N, T, Args...>
	{
		using tuple_type = typename get_tuple_type<N - 1, Args...>::tuple_type;
	};

	template<typename T, typename...Args>
	struct get_tuple_type<0, T, Args...>
	{
		using tuple_type = xmh::tuple<T, Args...>;
	};

	template<std::size_t N, typename...Args>
	using get_tuple_type_t = typename get_tuple_type<N, Args...>::tuple_type;

	template<std::size_t N, typename Tuple>
	struct get_tuple_element
	{

	};

	template<std::size_t N, typename...Args>
	struct get_tuple_element<N, xmh::tuple<Args...>>
	{
		using type = typename get_tuple_type_t<N, Args...>::element_type;
	};

	template<std::size_t N, typename Tuple>
	using get_tuple_element_t = typename get_tuple_element<N, Tuple>::type;


	template<typename T, typename...Args>
	struct find_type_for_tuple
	{

	};

	template<typename T, typename U, typename...Args>
	struct find_type_for_tuple<T, U, Args...>
	{
		using tuple_type = typename find_type_for_tuple<T, Args...>::tuple_type;
	};

	template<typename T, typename...Args>
	struct find_type_for_tuple<T, T, Args...>
	{
		using tuple_type = xmh::tuple<T, Args...>;
	};


	template<std::size_t N, typename...Args>
	constexpr auto get0(xmh::tuple<Args...>& tp)-> typename std::remove_reference<typename get_tuple_type<N, Args...>::tuple_type::element_type>::type&
	{
		using tuple_type = typename get_tuple_type<N, Args...>::tuple_type;
		return static_cast<tuple_type&>(tp).get();
	}
	template<typename T, typename...Args>
	constexpr auto get1(xmh::tuple<Args...>& tp) ->typename std::remove_reference<typename find_type_for_tuple<T, Args...>::tuple_type::element_type>::type&
	{
		using tuple_type = typename find_type_for_tuple<T, Args...>::tuple_type;
		return static_cast<tuple_type&>(tp).get();
	}

	template<std::size_t N, typename Tuple>
	constexpr auto get(Tuple&& tp)-> decltype(get0<N>(tp))
	{
		return get0<N>(tp);
	}

	template<typename T, typename Tuple>
	constexpr auto get(Tuple&& tp) -> decltype(get1<T>(tp))
	{
		return get1<T>(tp);
	}
}
