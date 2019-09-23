#pragma once
#include <iostream>
#include <utility>
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

		template<typename U,typename...Params>
		constexpr tuple<T, Args...>(U&& v, Params&&...args) : tuple<Args...>(std::forward<Args>(args)...), value(std::forward<U>(v))
		{

		}

		constexpr tuple(tuple const& tp): tuple<Args...>(tp), value(std::forward<typename tuple::element_type>(tp.value))
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
	constexpr auto get0(xmh::tuple<Args...>& tp)-> typename get_tuple_type<N, Args...>::tuple_type::element_type&
	{
		using tuple_type = typename get_tuple_type<N, Args...>::tuple_type;
		return static_cast<tuple_type&>(tp).get();
	}
	template<typename T, typename...Args>
	constexpr auto get1(xmh::tuple<Args...>& tp) ->typename find_type_for_tuple<T, Args...>::tuple_type::element_type&
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

	template<typename...Args, typename...Args2, std::size_t...Indexs, std::size_t...Indexs2>
	constexpr auto tuple_cat_help0(xmh::tuple<Args...>& tp, xmh::tuple<Args2...>& tp1, std::index_sequence<Indexs...>  , std::index_sequence<Indexs2...>)->xmh::tuple<Args..., Args2...>
	{
		return xmh::tuple<Args..., Args2...>(xmh::get<Indexs>(tp)..., xmh::get<Indexs2>(tp1)...);
	}

	template<typename...Args,typename...Args2>
	constexpr auto tuple_cat_help(xmh::tuple<Args...>& tp, xmh::tuple<Args2...>& tp1)->xmh::tuple<Args...,Args2...>
	{
		return tuple_cat_help0(tp, tp1, std::make_index_sequence<sizeof...(Args)>{}, std::make_index_sequence<sizeof...(Args2)>{});
	}

	template<typename Tuple,typename Tuple1>
	constexpr auto tuple_cat(Tuple&& tp, Tuple1&& tp1) ->decltype(tuple_cat_help(tp, tp1))
	{
		return tuple_cat_help(tp, tp1);
	}
}
