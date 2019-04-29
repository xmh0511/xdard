#pragma once
#include <tuple>
namespace xmh {
	template<typename Function, typename U = void>
	struct function_traist
	{

	};

	template<typename Ret, typename...Args>
	struct function_traist<Ret(*)(Args...), void>
	{
		using args_tuple = std::tuple<Args...>;
		using ret_type = Ret;
		using object_type = void*;
		constexpr static std::size_t size = sizeof...(Args);
	};

	template<typename Ret, typename Class, typename...Args>
	struct function_traist<Ret(Class::*)(Args...), void>
	{
		using args_tuple = std::tuple<Args...>;
		using ret_type = Ret;
		using object_type = Class * ;
		constexpr static std::size_t size = sizeof...(Args);
	};

	template<typename Ret, typename Class, typename...Args>
	struct function_traist<Ret(Class::*)(Args...) const, void>
	{
		using args_tuple = std::tuple<Args...>;
		using ret_type = Ret;
		constexpr static std::size_t size = sizeof...(Args);
	};

	template<typename Function>
	struct function_traist<Function, std::void_t<decltype(&Function::operator())>> :public function_traist<decltype(&Function::operator())>
	{
		using object_type = void*;
	};

	template<typename...Args>
	class Currying
	{

	};

	template<typename Function, typename T, typename...Params>
	class Currying<Function, T, Params...> :public Currying<Function, Params...>
	{
	public:
		Currying(Function const& function) :Currying<Function, Params...>(function)
		{

		}
		template<typename Class>
		Currying(Function const& function, Class* ptr) : Currying<Function, Params...>(function, ptr)
		{

		}
	public:
		Currying<Function, Params...>& operator()(T v)
		{
			std::get<(function_traist<Function>::size - sizeof...(Params) -1)>(this->arguments_) = v;
			return static_cast<Currying<Function, Params...>&>(*this);
		}
	};

	template<typename Function, typename T>
	class Currying<Function, T>
	{
	public:
		Currying(Function const& function) :function_(function), object_(nullptr)
		{

		}
		template<typename Class>
		Currying(Function const& function, Class* ptr) : function_(function), object_(ptr)
		{

		}
	public:
		typename function_traist<Function>::ret_type operator()(T v)
		{
			std::get<(function_traist<Function>::size - 1)>(this->arguments_) = v;
			return apply(std::make_index_sequence<(function_traist<Function>::size)>{});
		}
	private:
		template<std::size_t...Indexs>
		typename function_traist<Function>::ret_type apply(std::index_sequence<Indexs...>)
		{
			return apply_is_obj(object_, std::get<Indexs>(arguments_)...);
		}

		template<typename T, typename...Arguments>
		std::enable_if_t<std::is_same_v<T, void*>, typename function_traist<Function>::ret_type> apply_is_obj(T, Arguments&&...args)
		{
			return function_(args...);
		}

		template<typename T, typename...Arguments>
		std::enable_if_t<!std::is_same_v<T, void*>, typename function_traist<Function>::ret_type> apply_is_obj(T, Arguments&&...args)
		{
			if (object_ == nullptr) {
				throw 0;
			}
			return (object_->*function_)(args...);
		}
	protected:
		typename function_traist<Function>::args_tuple arguments_;
	private:
		Function function_;
		typename function_traist<Function>::object_type object_;
	};

	template<typename Function, typename...Args>
	auto to_currying_help(Function&& function, std::tuple<Args...>&&)
	{
		using ftype = std::remove_reference_t<Function>;
		return Currying<ftype, Args...>(std::forward<Function>(function));
	}

	template<typename Function>
	auto to_currying(Function&& function)
	{
		using ftype = std::remove_reference_t<Function>;
		return to_currying_help(std::forward<Function>(function), typename function_traist<ftype>::args_tuple{});
	}

	template<typename T, typename Function, typename...Args>
	auto to_currying_help(Function&& function, std::tuple<Args...>&&, T&& obj, std::enable_if_t<std::is_pointer_v<std::remove_reference_t<T>>, void*> = 0)
	{
		using ftype = std::remove_reference_t<Function>;
		return Currying<ftype, Args...>(std::forward<Function>(function), obj);
	}

	template<typename T, typename Function, typename...Args>
	auto to_currying_help(Function&& function, std::tuple<Args...>&&, T&& obj, std::enable_if_t<!std::is_pointer_v<std::remove_reference_t<T>>, void*> = 0)
	{
		using ftype = std::remove_reference_t<Function>;
		return Currying<ftype, Args...>(std::forward<Function>(function), &obj);
	}

	template<typename Function, typename Class>
	auto to_currying(Function&& function, Class&& obj)
	{
		using ftype = std::remove_reference_t<Function>;
		return to_currying_help(function, typename function_traist<ftype>::args_tuple{}, std::forward<Class>(obj));
	}
}