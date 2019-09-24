#pragma once
#include <type_traits>
namespace xmh {
	template<typename...Args>
	class tuple;

	template<std::size_t N, typename T>
	struct tuple_transform {

	};

	template<std::size_t N, typename T, typename...Args>
	struct tuple_transform<N, tuple<T, Args...>> {
		using type = typename tuple_transform<N - 1, tuple<Args...>>::type;
	};

	template<typename T, typename...Args>
	struct tuple_transform<0, tuple<T, Args...>> {
		using type = tuple<T, Args...>;
	};

	template<std::size_t N, typename T>
	struct tuple_element {

	};

	template<std::size_t N, typename T, typename...Args>
	struct tuple_element<N, tuple<T, Args...>> {
		using type = typename tuple_element<N - 1, tuple<Args...>>::type;
	};

	template<typename T, typename...Args>
	struct tuple_element<0, tuple<T, Args...>> {
		using type = T;
	};

	template<std::size_t N, typename T>
	using tuple_element_t = typename tuple_element<N, T>::type;

	template<typename T, typename U>
	struct tuple_transform_ {};

	template<typename T, typename U, typename...Args>
	struct tuple_transform_<T, tuple<U, Args...>> {
		using type = typename tuple_transform_<T, tuple<Args...>>::type;
	};
	template<typename T, typename...Args>
	struct tuple_transform_<T, tuple<T, Args...>> {
		using type = tuple<T, Args...>;
	};

	template<typename...Args>
	class tuple {};


	template<typename T, typename...Args>
	class tuple<T, Args...> :public tuple<Args...> {
		template<std::size_t N, typename...Parameters>
		friend constexpr typename tuple_element<N, tuple<Parameters...>>::type& get(tuple<Parameters...>&);
		template<std::size_t N, typename...Parameters>
		friend constexpr typename tuple_element<N, tuple<Parameters...>>::type& get(tuple<Parameters...>&&);

		template<typename P0, typename...P1>
		friend constexpr typename tuple_transform_<P0, tuple<P1...>>::type::type& get(tuple<P1...>&);

		template<typename P0, typename...P1>
		friend constexpr typename tuple_transform_<P0, tuple<P1...>>::type::type& get(tuple<P1...>&&);
	public:
		using type = T;
	public:
		constexpr tuple<T, Args...>() = default;
		template<typename P0, typename...Params>
		tuple<T, Args...>(P0&& p0, Params&& ...args) : tuple<Args...>(std::forward<Params>(args)...), value_(std::forward<P0>(p0)) {

		}
		tuple<T, Args...>(tuple<T, Args...> const& v) : tuple<Args...>(v), value_(v.value_) {

		}
	public:
		tuple<T, Args...>& operator=(tuple<T, Args...> const& v) {
			tuple<Args...>::operator=(v);
			value_ = v.value_;
			return *this;
		}
	private:
		T value_;
	};

	template<>
	class tuple<> {
	public:
		constexpr tuple() = default;
		constexpr tuple(tuple const&) = default;
	};


	template<std::size_t N, typename...Args>
	constexpr typename tuple_element<N, tuple<Args...>>::type& get(tuple<Args...>& v) {
		using type = typename tuple_transform<N, tuple<Args...>>::type;
		return (static_cast<type&>(v)).value_;
	}

	template<std::size_t N, typename...Args>
	constexpr typename tuple_element <N, tuple<Args...>>::type& get(tuple<Args...>&& v) {
		return get(v);
	}

	template<typename P0, typename...P1>
	constexpr typename tuple_transform_<P0, tuple<P1...>>::type::type& get(tuple<P1...>& v) {
		using type = typename tuple_transform_<P0, tuple<P1...>>::type;
		return (static_cast<type&>(v)).value_;
	}

	template<typename P0, typename...P1>
	constexpr typename tuple_transform_<P0, tuple<P1...>>::type::type& get(tuple<P1...>&& v) {
		return get<P0>(v);
	}
}