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


	template<typename T>
	struct tuple_size {

	};

	template<typename...Args>
	struct tuple_size<tuple<Args...>> {
		constexpr static std::size_t value = sizeof...(Args);
	};

	template<typename T>
	constexpr std::size_t tuple_size_v = tuple_size<T>::value;

	template<typename T>
	struct is_tuple {
		constexpr static bool value = false;
	};

	template<typename...Args>
	struct is_tuple<tuple<Args...>> {
		constexpr static bool value = true;
	};

	template<typename...Args>
	class tuple {};


	template<typename T, typename...Args>
	class tuple<T, Args...> :private tuple<Args...> {
		template<std::size_t N, typename...Parameters>
		friend constexpr typename tuple_element<N, tuple<Parameters...>>::type& get(tuple<Parameters...>&);

		template<std::size_t N, typename...Parameters>
		friend constexpr typename tuple_element<N, tuple<Parameters...>>::type const& get(tuple<Parameters...> const& v);

		template<std::size_t N, typename...Parameters>
		friend constexpr typename tuple_element<N, tuple<Parameters...>>::type& get(tuple<Parameters...>&&);

		template<typename P0, typename...P1>
		friend constexpr typename tuple_transform_<P0, tuple<P1...>>::type::value_type& get(tuple<P1...>&);

		template<typename P0, typename...P1>
		friend constexpr typename tuple_transform_<P0, tuple<P1...>>::type::value_type const& get(tuple<P1...> const& v);

		template<typename P0, typename...P1>
		friend constexpr typename tuple_transform_<P0, tuple<P1...>>::type::value_type& get(tuple<P1...>&&);

	public:
		using value_type = T;
	public:
		constexpr tuple<T, Args...>() = default;
		template<typename P0, typename...Params, typename U = typename std::enable_if<!is_tuple<typename std::remove_cv<typename std::remove_reference<P0>::type>::type>::value>::type>
		tuple<T, Args...>(P0&& p0, Params&& ...args) : tuple<Args...>(std::forward<Params>(args)...), value_(std::forward<P0>(p0)) {

		}
		tuple<T, Args...>(tuple<T, Args...> const& v) : tuple<Args...>(static_cast<tuple<Args...> const&>(v)), value_(v.value_) {

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
	constexpr typename tuple_element<N, tuple<Args...>>::type const& get(tuple<Args...> const& v) {
		using type = typename tuple_transform<N, tuple<Args...>>::type;
		return (static_cast<type const&>(v)).value_;
	}

	template<std::size_t N, typename...Args>
	constexpr typename tuple_element <N, tuple<Args...>>::type& get(tuple<Args...>&& v) {
		return get(v);
	}

	template<typename P0, typename...P1>
	constexpr typename tuple_transform_<P0, tuple<P1...>>::type::value_type& get(tuple<P1...>& v) {
		using type = typename tuple_transform_<P0, tuple<P1...>>::type;
		return (static_cast<type&>(v)).value_;
	}

	template<typename P0, typename...P1>
	constexpr typename tuple_transform_<P0, tuple<P1...>>::type::value_type const& get(tuple<P1...> const& v) {
		using type = typename tuple_transform_<P0, tuple<P1...>>::type;
		return (static_cast<type const&>(v)).value_;
	}

	template<typename P0, typename...P1>
	constexpr typename tuple_transform_<P0, tuple<P1...>>::type::type& get(tuple<P1...>&& v) {
		return get<P0>(v);
	}

	template<std::size_t...N>
	struct index_sequeue {

	};

	template<std::size_t N, std::size_t...Indexs>
	struct index_sequeue<N, Indexs...> {
		using type = typename index_sequeue<N - 1, N - 1, Indexs...>::type;
	};

	template<std::size_t...Indexs>
	struct index_sequeue<0, Indexs...> {
		using type = index_sequeue<Indexs...>;
	};

	template<typename Y, typename T, typename U, std::size_t...Indexs, std::size_t...Indexs1>
	constexpr Y tuple_cat_(T& v0, U& v1, index_sequeue<Indexs...>, index_sequeue<Indexs1...>) {
		 return Y{ get<Indexs>(v0)...,get<Indexs1>(v1)... };
	}

	template<typename...Args, typename...Params>
	constexpr tuple<Args..., Params...> tuple_cat(tuple<Args...> const& v0, tuple<Params...> const& v1) {
		 return tuple_cat_<tuple<Args..., Params...>>(v0, v1, typename index_sequeue<sizeof...(Args)>::type{}, typename index_sequeue<sizeof...(Params)>::type{});
	}

	template<typename...Args>
	tuple<Args...> make_tuple(Args&& ...args) {
		return tuple<Args...>(std::forward<Args>(args)...);
	}

}