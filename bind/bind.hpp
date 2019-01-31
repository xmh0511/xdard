#pragma once
#include <iostream>
#include "tuple.hpp"
#include "function_traits.hpp"
namespace xmh {
	template<std::size_t...Indexs>
	struct index_sequeue
	{

	};

	template<std::size_t N,std::size_t...Indexs>
	struct index_sequeue<N, Indexs...>
	{
		using type = typename index_sequeue<N - 1, N - 1, Indexs...>::type;
	};

	template<std::size_t...Indexs>
	struct index_sequeue<0, Indexs...>
	{
		using type = index_sequeue<Indexs...>;
	};

	template<std::size_t N>
	struct placehold {};

	template<std::size_t N,std::size_t Max>
	struct params_filter
	{
		template<typename T,typename Tuple>
		static constexpr auto transform(T&& t, Tuple&& tp)->T&&
		{
			return std::forward<T>(t);
		}

		template<std::size_t Index, typename Tuple>
		static constexpr auto transform(placehold<Index>,Tuple&& tp)->get_tuple_element_t<Index-1, std::remove_reference_t<Tuple>>
		{
			return xmh::get<Index-1>(tp);
		}

		template<typename ParamsTuple,typename Tuple,typename ArgumentTuple>
		static constexpr auto dispatch(ParamsTuple&& params, Tuple&& tp, ArgumentTuple&& argument)
		{
			auto&& v = transform(xmh::get<N>(params), std::forward<Tuple>(tp));
			using vtype = decltype(transform(xmh::get<N>(params), std::forward<Tuple>(tp)));
			auto packages = xmh::tuple_cat(argument, xmh::tuple<vtype>(std::forward<vtype>(v)));
			return params_filter<N + 1, Max>::template dispatch(std::forward<ParamsTuple>(params), std::forward<Tuple>(tp), std::move(packages));
		}
	};

	template<std::size_t Max>
	struct params_filter<Max, Max>
	{
		template<typename ParamsTuple, typename Tuple, typename ArgumentTuple>
		static constexpr auto dispatch(ParamsTuple&& params, Tuple&& tp, ArgumentTuple&& argument)
		{
			return argument;
		}
	};

	template<typename Function,typename...Args>
	class Binder
	{
	public:
		using ret = typename function_traits<Function>::ret;
		using params_type_tuple = typename function_traits<Function>::params_type_tuple;
	public:
		template<typename Method,typename...Params>
		constexpr Binder(Method&& function, Params&&...args):func_(function), bind_arguments_(std::forward<Args>(args)...)
		{
			//std::cout << typeid(xmh::tuple<Args...>).name() << std::endl;
		}
	public:
		template<std::size_t...Indexs,typename Tuple>
		ret call(index_sequeue<Indexs...>, Tuple&& argus)
		{
			return func_(static_cast<xmh::get_tuple_element_t<Indexs, params_type_tuple>>(xmh::get<Indexs>(argus))...);
		}

		template<typename...Params>
		ret operator()(Params&&...params)
		{
			auto argument = params_filter<0, sizeof...(Args)>::template dispatch(bind_arguments_, xmh::tuple<Params&...>(params...), xmh::tuple<>{});
			//std::cout << typeid(argument).name() << std::endl;
			return call(typename index_sequeue<sizeof...(Args)>::type{}, argument);
		}
	private:
		Function func_;
		xmh::tuple<Args...> bind_arguments_;
	};


	template<typename Ret,typename Class,typename...Params, typename...Args>
	class Binder<Ret(Class::*)(Params...), Args...>
	{
	public:
		using ret = Ret;
		using Function = Ret(Class::*)(Params...);
		using params_type_tuple = typename function_traits<Function>::params_type_tuple;
	public:
		template<typename Method,typename Self, typename...Params2,typename = std::enable_if_t<std::is_pointer_v<std::remove_reference_t<Self>>>>
		constexpr Binder(int,Method&& function, Self&& self, Params2&&...args) :func_(function), bind_arguments_(std::forward<Params2>(args)...), self(*self)
		{
			//std::cout << typeid(xmh::tuple<Args...>).name() << std::endl;
		}

		template<typename Method, typename Self, typename...Params2, typename = std::enable_if_t<!std::is_pointer_v<std::remove_reference_t<Self>>>>
		constexpr Binder(float,Method&& function, Self&& self, Params2&&...args) : func_(function), bind_arguments_(std::forward<Params2>(args)...), self(std::forward<Self>(self))
		{
			//std::cout<<"save tuple" << typeid(xmh::tuple<Args...>).name()<<"    "<<xmh::get<0>(bind_arguments_) << std::endl;
		}

	public:
		template<std::size_t...Indexs, typename Tuple>
		ret call(index_sequeue<Indexs...>, Tuple&& argus)
		{
			return (self.*func_)(static_cast<xmh::get_tuple_element_t<Indexs, params_type_tuple>>(xmh::get<Indexs>(argus))...);
		}

		template<typename...Params2>
		ret operator()(Params2&&...params)
		{
			auto argument = params_filter<0, sizeof...(Args)>::template dispatch(bind_arguments_, xmh::tuple<Params2&...>(params...), xmh::tuple<>{});
			//std::cout << typeid(argument).name() << std::endl;
			return call(typename index_sequeue<sizeof...(Args)>::type{}, argument);
		}
	private:
		Function func_;
		xmh::tuple<Args...> bind_arguments_;
		Class& self;
	};
	//template<std::size_t N,std::size_t Max>
	//struct convertion
	//{
	//	template<typename T,typename ArgusType,typename Package,typename Tuple>
	//	static constexpr auto dispatch(T&& t, ArgusType&& tp, Package&& tp1, Tuple&&)
	//	{
	//		using Type = xmh::get_tuple_element_t<N, std::remove_reference_t<ArgusType>>;

	//	}
	//};

	template<std::size_t N, std::size_t Max,typename...T>
	struct convertion
	{

	};

	template<std::size_t N, std::size_t Max, typename T, typename ArgusType, typename Package,typename...Complete>
	struct convertion<N,Max,T, ArgusType, Package, Complete...>
	{
		using type = typename convertion<N + 1, Max, xmh::get_tuple_element_t<N + 1, Package>, ArgusType, Package, Complete..., xmh::get_tuple_element_t<N, ArgusType>>::type;
	};

	template<std::size_t N, std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<N, Max, xmh::placehold<Index>, ArgusType, Package, Complete...>
	{
		using type = typename convertion<N + 1, Max, xmh::get_tuple_element_t<N + 1, Package>, ArgusType, Package, Complete..., xmh::placehold<Index>>::type;
	};

	template<std::size_t N, std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<N, Max, xmh::placehold<Index>&&, ArgusType, Package, Complete...>
	{
		using type = typename convertion<N + 1, Max, xmh::get_tuple_element_t<N + 1, Package>, ArgusType, Package, Complete..., xmh::placehold<Index>>::type;
	};

	template<std::size_t N, std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<N, Max, xmh::placehold<Index>&, ArgusType, Package, Complete...>
	{
		using type = typename convertion<N + 1, Max, xmh::get_tuple_element_t<N + 1, Package>, ArgusType, Package, Complete..., xmh::placehold<Index>>::type;
	};

	template<std::size_t Max, typename T, typename ArgusType, typename Package, typename...Complete>
	struct convertion<Max, Max, T, ArgusType, Package, Complete...>
	{
		using type = xmh::tuple<Complete..., xmh::get_tuple_element_t<Max, ArgusType>>;
	};

	template<std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<Max, Max, xmh::placehold<Index>, ArgusType, Package, Complete...>
	{
		using type = xmh::tuple<Complete..., xmh::placehold<Index>>;
	};

	template<std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<Max, Max, xmh::placehold<Index>&&, ArgusType, Package, Complete...>
	{
		using type = xmh::tuple<Complete..., xmh::placehold<Index>>;
	};

	template<std::size_t Max, std::size_t Index, typename ArgusType, typename Package, typename...Complete>
	struct convertion<Max, Max, xmh::placehold<Index>&, ArgusType, Package, Complete...>
	{
		using type = xmh::tuple<Complete..., xmh::placehold<Index>>;
	};

	template<typename Tuple,std::size_t...Indexs, typename Function,typename Argument>
	auto bind_(Function&& function, index_sequeue<Indexs...>, Argument&& tp)
	{
		return Binder<std::remove_reference_t<Function>, xmh::get_tuple_element_t<Indexs, Tuple>...>(std::forward<Function>(function),xmh::get<Indexs>(tp)...);
	}

	template<typename Tuple, std::size_t...Indexs, typename Function,typename Self, typename Argument>
	auto bind_class(Function&& function, Self&& self,index_sequeue<Indexs...>, Argument&& tp)
	{
		//std::cout << typeid(typename Binder<std::remove_reference_t<Function>, xmh::get_tuple_element_t<Indexs, Tuple>...>::Function).name() << std::endl;
		return Binder<std::remove_reference_t<Function>, xmh::get_tuple_element_t<Indexs, Tuple>...>(0,std::forward<Function>(function),std::forward<Self>(self), xmh::get<Indexs>(tp)...);
	}

	template<typename Function,typename...Args>
	auto bind(Function&& function, Args&&...args)
	{
		using params_tuple = typename function_traits<std::remove_reference_t<Function>>::params_type_tuple;
		constexpr auto size = function_traits<std::remove_reference_t<Function>>::args_size;
		using package_type = xmh::tuple<Args&&...>;
		using bind_args_type = typename convertion<0, size - 1, xmh::get_tuple_element_t<0, package_type>, params_tuple, package_type>::type;
		//std::cout << typeid(package_type).name() << std::endl;
	    return bind_<bind_args_type>(std::forward<Function>(function), typename index_sequeue<size>::type{}, package_type(std::forward<Args>(args)...));
	}


	template<typename Ret,typename Class,typename...Params,typename Self, typename...Args>
	auto bind(Ret(Class::*function)(Params...), Self&& self, Args&&...args)
	{
		using params_tuple = xmh::tuple<Params...>;
		constexpr auto size = sizeof...(Params);
		using package_type = xmh::tuple<Args&&...>;
		using bind_args_type = typename convertion<0, size - 1, xmh::get_tuple_element_t<0, package_type>, params_tuple, package_type>::type;
		//std::cout << typeid(bind_args_type).name() << std::endl;
		return bind_class<bind_args_type>(function, std::forward<Self>(self),typename index_sequeue<size>::type{}, package_type(std::forward<Args>(args)...));
	}
}