#pragma once
namespace xmh {
	template<typename...Args>
	class tuple;
	template<std::size_t N, std::size_t Max, typename T>
	struct downtuple {

	};

	template<std::size_t N, std::size_t Max, typename T, typename...Args>
	struct downtuple<N, Max, tuple<T, Args...>> {
		using type = typename downtuple<N + 1, Max, tuple<Args...>>::type;
	};

	template<std::size_t Max, typename T, typename...Args>
	struct downtuple<Max, Max, tuple<T, Args...>> {
		using type = tuple<T, Args...>;
	};

	template<typename T, typename U>
	struct downtuple_type {};

	template<typename T, typename U, typename...Args>
	struct downtuple_type<T, tuple<U, Args...>> {
		using type = typename downtuple_type<T, tuple<Args...>>::type;
	};
	template<typename T, typename...Args>
	struct downtuple_type<T, tuple<T, Args...>> {
		using type = tuple<T, Args...>;
	};

	template<typename...Args>
	class tuple {};


	template<typename T, typename...Args>
	class tuple<T, Args...> :public tuple<Args...> {
		template<std::size_t N, typename...Parameters>
		friend typename downtuple<0, N, tuple<Parameters...>>::type::type& get(tuple<Parameters...>&);
		template<std::size_t N, typename...Parameters>
		friend typename downtuple<0, N, tuple<Parameters...>>::type::type& get(tuple<Parameters...>&&);

		template<typename P0, typename...P1>
		friend typename downtuple_type<P0, tuple<P1...>>::type::type& get(tuple<P1...>&);

		template<typename P0, typename...P1>
		friend typename downtuple_type<P0, tuple<P1...>>::type::type& get(tuple<P1...>&&);
	public:
		using type = T;
	public:
		tuple<T, Args...>() = default;
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
		tuple() = default;
		tuple(tuple const&) = default;
		template<typename...Args>
		tuple(Args&& ...) {

		}
	};


	template<std::size_t N, typename...Args>
	typename downtuple<0, N, tuple<Args...>>::type::type& get(tuple<Args...>& v) {
		using type = typename downtuple<0, N, tuple<Args...>>::type;
		return (static_cast<type&>(v)).value_;
	}

	template<std::size_t N, typename...Args>
	typename downtuple<0, N, tuple<Args...>>::type::type& get(tuple<Args...>&& v) {
		return get(v);
	}

	template<typename P0, typename...P1>
	typename downtuple_type<P0, tuple<P1...>>::type::type& get(tuple<P1...>& v) {
		using type = typename downtuple_type<P0, tuple<P1...>>::type;
		return (static_cast<type&>(v)).value_;
	}

	template<typename P0, typename...P1>
	typename downtuple_type<P0, tuple<P1...>>::type::type& get(tuple<P1...>&& v) {
		return get<P0>(v);
	}
}