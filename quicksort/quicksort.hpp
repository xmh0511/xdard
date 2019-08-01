#pragma once
namespace xmh {
	template<typename T, typename U = void>
	struct traist_vector {

	};
	template<typename T, std::size_t Size>
	struct traist_vector<T[Size], void> {
		template<typename Y>
		static std::size_t length(Y&& y) {
			return Size;
		}
	};
	template<typename T>
	struct traist_vector<T, std::void_t<decltype(std::declval<T>().size())>> {
		template<typename Y>
		static std::size_t length(Y&& y) {
			return y.size();
		}
	};
	template<typename Function>
	class quicksort {
	public:
		quicksort(Function& v) :Value_(v) {

		}
	public:
		template<typename T, typename U>
		void quick_sort(U& arr, T* begin, T* end) {
			if (begin < end) {
				auto middle = *begin;
				auto left = begin;
				auto right = end;
				while (left < right) {
					while (left < right) {
						if (Value_(*right) < Value_(middle)) {
							*left = *right;
							break;
						}
						--right;
					}
					while (left < right) {
						if (Value_(*left) > Value_(middle)) {
							*right = *left;
							break;
						}
						++left;
					}
				}
				*left = middle;
				quick_sort(arr, begin, left - 1);
				quick_sort(arr, left + 1, end);
			}
		}
	private:
		Function& Value_;
	};
	template<typename T, typename U = std::void_t<decltype(std::declval<std::decay_t<std::remove_reference_t<T>>>()[0])>>
	void quick_sort(T& vector) {
		using type = std::remove_reference_t<T>;
		auto size = traist_vector<type>::template length(vector);
		auto begin = &vector[0];
		auto end = &vector[size - 1];
		auto value = [](auto& v) {return v; };
		quicksort<decltype(value)>(value).quick_sort(vector, begin, end);
	}


	template<typename T, typename Value, typename U = std::void_t<decltype(std::declval<std::decay_t<std::remove_reference_t<T>>>()[0])>>
	void quick_sort(T& vector, Value&& function) {
		using type = std::remove_reference_t<T>;
		auto size = traist_vector<type>::template length(vector);
		auto begin = &vector[0];
		auto end = &vector[size - 1];
		quicksort<std::remove_reference_t<Value>>(function).quick_sort(vector, begin, end);
	}
}