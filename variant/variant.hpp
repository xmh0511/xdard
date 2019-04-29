#pragma once
#include <tuple>
#include <string>
namespace xmh
{
	template<int...Indexs>
	struct index_seque_type
	{

	};
	template<int Count, int differ, int To, int...Indexs>
	struct index_seque_from_to_help
	{
		using type = typename index_seque_from_to_help<Count + 1, differ, To, To - Count - 1, Indexs...>::type;
	};
	template<int Count, int To, int...Indexs>
	struct index_seque_from_to_help<Count, Count, To, Indexs...>
	{
		using type = index_seque_type<Indexs...>;
	};
	template<int Count, int To, int...Indexs>
	struct index_seque_from_to_help<Count, -1, To, Indexs...>
	{
		using type = index_seque_type<To>;
	};

	template<int To, int...Indexs>
	struct index_seque_from_to_help<0, 0, To, Indexs...>
	{
		using type = index_seque_type<0>;
	};

	template<int From, int To>
	struct index_seque_from_to
	{
		using type = typename index_seque_from_to_help<0, To - From, To>::type;
	};

	template<int From>
	struct index_seque_from_to<From, From>
	{
		using type = index_seque_type<-1>;
	};
	///get tuple type from x to y


	template<typename Tuple, typename T, typename U, int...Indexs, int...Indexs2>
	constexpr auto get_type_from_tuple(Tuple&&, T&&, U&&, index_seque_type<Indexs...>, index_seque_type<Indexs2...>)
	{
		return std::tuple<std::tuple_element_t<Indexs, std::remove_reference_t<Tuple>>..., std::remove_reference_t<T>, std::remove_reference_t<U>, std::tuple_element_t<Indexs2, std::remove_reference_t<Tuple>>...>{};
	}

	template<typename Tuple, typename T, typename U, int...Indexs2>
	constexpr auto get_type_from_tuple(Tuple&&, T&&, U&&, index_seque_type<-1>, index_seque_type<Indexs2...>)
	{
		//std::cout << typeid(e).name() << std::endl;
		return std::tuple<std::remove_reference_t<T>, std::remove_reference_t<U>, std::tuple_element_t<Indexs2, std::remove_reference_t<Tuple>>...>{};
		//return a;
	}

	template<typename Tuple, typename T, typename U, int...Indexs>
	constexpr auto get_type_from_tuple(Tuple&&, T&&, U&&, index_seque_type<Indexs...>, index_seque_type<-1>)
	{
		return std::tuple<std::tuple_element_t<Indexs, std::remove_reference_t<Tuple>>..., std::remove_reference_t<T>, std::remove_reference_t<U>>{};
	}

	template<typename Tuple, typename T, typename U>
	constexpr auto get_type_from_tuple(Tuple&&, T&&, U&&, index_seque_type<-1>, index_seque_type<-1>)
	{
		return std::tuple<std::remove_reference_t<T>, std::remove_reference_t<U>>{};
	}

	template<bool F>
	struct IF_True
	{

	};

	template<>
	struct IF_True<true>
	{
		template<typename Tuple, typename T, typename U, typename A, typename B>
		constexpr static auto help(Tuple&& a, T&& b, U&& c, A&& d, B&& e)
		{
			return get_type_from_tuple(std::forward<Tuple>(a), std::forward<U>(c), std::forward<T>(b), std::forward<A>(d), std::forward<B>(e));
		}
	};
	template<>
	struct IF_True<false>
	{
		template<typename Tuple, typename T, typename U, typename A, typename B>
		constexpr static auto help(Tuple&& a, T&& b, U&& c, A&& d, B&& e)
		{
			return get_type_from_tuple(std::forward<Tuple>(a), std::forward<T>(b), std::forward<U>(c), std::forward<A>(d), std::forward<B>(e));
		}
	};

	template<std::size_t N>
	struct args_more_one
	{
		constexpr static bool value = N > 1 ? true : false;
	};

	template<int N>
	struct contain_type_in_argus:std::true_type
	{

	};

	template<>
	struct contain_type_in_argus<-1> :std::false_type
	{

	};

	template <int N, int Index>
	struct For0
	{
		template<typename Tuple>
		constexpr static auto do_action(Tuple&& tp)
		{
			using now_type = typename  std::tuple_element<Index, std::remove_reference_t<Tuple>>::type;
			using next_type = typename  std::tuple_element<Index + 1, std::remove_reference_t<Tuple>>::type;
			constexpr bool result = sizeof(now_type) > sizeof(next_type);
			//constexpr int from = (Index + 2) == std::tuple_size<Tuple>::value? (Index+1): (Index + 2);
			auto tuple = IF_True<result> ::template help(std::forward<Tuple>(tp), now_type{}, next_type{}, typename index_seque_from_to<0, Index>::type{}, typename index_seque_from_to<Index + 2, std::tuple_size<std::remove_reference_t<Tuple>>::value>::type{});
			//std::cout << typeid(tuple).name() << std::endl;
			return For0<N, Index + 1>::template do_action(tuple);

		}
	};

	template <int N>
	struct For0<N, N>
	{
		template<typename Tuple>
		constexpr static auto do_action(Tuple&& tp)
		{
			return tp;
		}
	};

	template<int N, int Index>
	struct For1
	{
		template<typename Tuple>
		constexpr static auto do_action(Tuple&& tp)
		{
			constexpr auto args_size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
			auto new_tp = For0<args_size - 1, 0>::template do_action(std::forward<Tuple>(tp));
			return For1<N, Index + 1>::template do_action(new_tp);
		}
	};
	template<int N>
	struct For1<N, N>
	{
		template<typename Tuple>
		constexpr static auto do_action(Tuple&& tp)
		{
			return tp;
		}
	};
	template<typename...Args>
	struct sort_type
	{
		using type = decltype(For1<sizeof...(Args) - 1, 0>::template do_action(std::tuple<Args...>{}));
	};

	template<int N, typename T, typename...Args>
	struct find_type_of_index
	{
		//constexpr static int value = -1;
	};

	template<int N, typename T, typename U, typename...Args>
	struct find_type_of_index<N,T,U,Args...>
	{
		constexpr static int value = find_type_of_index<N + 1, T, Args...>::value;
	};

	template<int N, typename T, typename...Args>
	struct find_type_of_index<N, T, T, Args...>
	{
		constexpr static int value = N;
	};

	template<int N, typename T>
	struct find_type_of_index<N, T>
	{
		constexpr static int value = -1;
	};

	template<typename Tuple,typename T,typename...Args>
	struct find_type_of_variant
	{

	};



	template<typename Tuple, typename...Args>
	class _variant_interface_help
	{

	};

	template<typename Tuple, typename T, typename...Args>
	class _variant_interface_help<Tuple, T, Args...> :public _variant_interface_help<Tuple, Args...>
	{
	protected:
		using memory_ptr_type = std::tuple_element_t<std::tuple_size_v<Tuple>-1, Tuple>*;

		_variant_interface_help<Tuple, T, Args...>(memory_ptr_type ptr):memory_ptr(ptr), _variant_interface_help<Tuple, Args...>(ptr)
		{

		}
	public:
		operator T&()
		{
			return *((T*)memory_ptr);
		}

		std::string type_name()
		{
			return typeid(T).name();
		}
	private:
		memory_ptr_type memory_ptr;
	};

	template<typename Tuple, typename T>
	class _variant_interface_help<Tuple, T>
	{
		
		//template<int N, int Index>
		//friend struct Dynamic_Find;
	protected:
		using memory_ptr_type = std::tuple_element_t<std::tuple_size_v<Tuple>-1, Tuple>*;
		_variant_interface_help<Tuple, T>(memory_ptr_type ptr): memory_ptr(ptr)
		{

		};
	public:
		operator T&()
		{
			return *((T*)memory_ptr);
		}

		std::string type_name()
		{
			return typeid(T).name();
		}
	private:
		memory_ptr_type memory_ptr;
	};


	template<typename Tuple, typename T, typename U, typename...Args>
	struct find_type_of_variant<Tuple, T, U, Args...>
	{
		using type =typename  find_type_of_variant<Tuple, T, Args...>::type;
	};

	template<typename Tuple, typename T, typename...Args>
	struct find_type_of_variant<Tuple, T, T, Args...>
	{
		using type = _variant_interface_help<Tuple, T, Args...>;
	};

	template<typename Tuple, typename T, typename U>
	struct find_type_of_variant<Tuple, T, U>
	{
		using type = _variant_interface_help<Tuple, U>;
	};

	template<int N,int Index, typename Tuple,typename...Args>
	struct find_variant_base_by_index
	{
		
	};

	template<int N, int Index, typename Tuple,typename U, typename...Args>
	struct find_variant_base_by_index<N,Index,Tuple,U,Args...>
	{
		using type = typename find_variant_base_by_index<N, Index + 1, Tuple, Args...>::type;
	};

	template<int N, typename Tuple,typename T, typename...Args>
	struct find_variant_base_by_index<N,N,Tuple,T, Args...>
	{
		using type = _variant_interface_help<Tuple,T,Args...>;
	};

	template<int N,int Index>
	struct Dynamic_Find
	{
		template<typename Function,typename Tuple,int...Size>
		static auto call(int value,Function&& f, Tuple&& tp, index_seque_type<Size...>&& index_params)
		{
			if (Index == value) {
				using type = typename find_variant_base_by_index<Index, 0, Tuple, std::tuple_element_t<Size, Tuple>...>::type;
				f((type*)0);
			}
			return Dynamic_Find<N, Index + 1>::template call(value, std::forward<Function>(f), std::forward<Tuple>(tp), std::move(index_params));
		}
	};

	template<int N>
	struct Dynamic_Find<N,N>
	{
		template<typename Function, typename Tuple, int...Size>
		static auto call(int value, Function&& f, Tuple&& tp, index_seque_type<Size...>&& index_params)
		{
			if (N == value) {
				//using type = typename find_variant_base_by_index<N, 0, Tuple, std::tuple_element_t<Size, Tuple>...>::type;
				//f((type*)0);
			}
			return 0;
		}
	};


	template<typename Index,typename...Args>
	class variant_
	{

	};

	template<int...Indexs, typename...Args>
	class variant_<index_seque_type<Indexs...>, Args...>:public _variant_interface_help<typename sort_type<Args...>::type,std::tuple_element_t<Indexs, typename sort_type<Args...>::type>...>
	{
	public:
		using after_sort = typename sort_type<Args...>::type;
		using class_type = variant_<index_seque_type<Indexs...>, Args...>;
		using big_type = std::tuple_element_t<std::tuple_size_v<after_sort>-1, after_sort>;
		variant_<index_seque_type<Indexs...>, Args...>(): _variant_interface_help<typename sort_type<Args...>::type, std::tuple_element_t<Indexs, typename sort_type<Args...>::type>...>(&memory)
		{

		}

		variant_<index_seque_type<Indexs...>, Args...>(const class_type& obj) : _variant_interface_help<typename sort_type<Args...>::type, std::tuple_element_t<Indexs, typename sort_type<Args...>::type>...>(&memory), memory(obj.memory)
		{
			char buff[sizeof(big_type)] = {0};
			memcpy(&memory, buff, sizeof(big_type));
			//std::cout << "copy construct" << std::endl;
		}

		template<typename T, typename U = std::enable_if_t<contain_type_in_argus<find_type_of_index<0, T, Args...>::value>::value>>
		variant_<index_seque_type<Indexs...>, Args...>(T&& value): _variant_interface_help<typename sort_type<Args...>::type, std::tuple_element_t<Indexs, typename sort_type<Args...>::type>...>(&memory)
		{
			interface_call(value);
		}

		template<typename T>
		std::enable_if_t<contain_type_in_argus<find_type_of_index<0, T, Args...>::value>::value> interface_call(const T& value)
		{
			memcpy(&memory, &value, sizeof(T));
			set_index = find_type_of_index<0, T, Args...>::value;
			set_sort_index = find_type_of_index<0, T,std::tuple_element_t<Indexs,after_sort>...>::value;
		}

		template<typename T,typename U = std::enable_if_t<contain_type_in_argus<find_type_of_index<0, T, Args...>::value>::value>>
		class_type& operator=(const T& value)
		{
			//std::cout << "operator =" << contain_type_in_argus<find_type_of_index<0, T, Args...>::value>::value << std::endl;
			interface_call(value);
			return *this;
		}

		template<typename T>
		std::enable_if_t<contain_type_in_argus<find_type_of_index<0, T, Args...>::value>::value, std::remove_reference_t<std::remove_cv_t<T>>&> get()
		{
			using type = std::remove_reference_t<std::remove_cv_t<T>>;
			return *((type*)&memory);
		}

		unsigned int type_index()
		{
			return set_index;
		}

		std::string type_name()
		{
			std::string name_str;
			Dynamic_Find<sizeof...(Args), 0>::template call(set_sort_index, [this,&name_str](auto x) {
				//std::cout << typeid(std::remove_pointer_t<decltype(x)>).name() << std::endl;
				using type = std::remove_pointer_t<decltype(x)>;
				name_str = (static_cast<type*>(this))->type_name();
			}, after_sort{}, typename index_seque_from_to<0, sizeof...(Args)>::type{});
			return name_str;
		}

	private:
		std::tuple_element_t<std::tuple_size_v<after_sort>-1, after_sort> memory;
		unsigned int set_index = -1;
		unsigned int set_sort_index = -1;
	};

	template<typename...Args>
	using variant = variant_<typename index_seque_from_to<0, sizeof...(Args)>::type, Args...>;
};
