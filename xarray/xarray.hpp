#pragma once
#include <array>
#include <memory>
namespace xmh {
	template<std::size_t...Indexs>
	struct content {

	};
	template<typename T, typename U>
	struct traist_result {
		using params = U;
		using type = T;
	};
	template<typename...Args>
	struct traist_array {};

	template<typename T, std::size_t Size, std::size_t...Indexs>
	struct traist_array<T[Size], content<Indexs...>>
	{
		using type = typename traist_array<T, content<Indexs..., Size>>::type;
	};
	template<typename T, std::size_t...Indexs>
	struct traist_array<T, content<Indexs...>>
	{
		using type = traist_result<T, content<Indexs...>>;
	};

	template<typename T>
	struct calculat_size {};
	template<std::size_t N, std::size_t...Indexs>
	struct calculat_size<content<N, Indexs...>>
	{
		static constexpr std::size_t size = N * calculat_size<content<Indexs...>>::size;
	};
	template<>
	struct calculat_size <content<>>
	{
		static constexpr std::size_t size = 1;
	};


	template<std::size_t N, std::size_t Max, typename T>
	struct calculat_size_from_index
	{

	};

	template<std::size_t N, std::size_t Max, std::size_t I, std::size_t...Indexs>
	struct calculat_size_from_index<N, Max, content<I, Indexs...>>
	{
		static constexpr std::size_t value = I * calculat_size_from_index<N + 1, Max, content<Indexs...>>::value;
	};

	template<std::size_t Max, std::size_t I, std::size_t...Indexs>
	struct calculat_size_from_index<Max, Max, content<I, Indexs...>>
	{
		static constexpr std::size_t value = I;
	};

	template<typename Element, typename Package, typename Package2>
	class xarray_ {};
	template<typename Element, typename Package, std::size_t N, std::size_t...Indexs>
	class xarray_<Element, Package, content<N, Indexs...>> :public xarray_<Element, Package, content<Indexs...>>
	{
	public:
		xarray_() = default;
		xarray_(xarray_&& arr) :xarray_<Element, Package, content<Indexs...>>(std::move(arr))
		{

		}
	public:
		xarray_<Element, Package, content<Indexs...>>& operator[](std::size_t index)
		{
			this->check_over_range(index, N);
			this->index_ += index * (calculat_size<content<Indexs...>>::size);
			return static_cast<xarray_<Element, Package, content<Indexs...>>&>(*this);
		}

		xarray_<Element, Package, content<Indexs...>> const& operator[](std::size_t index) const
		{
			this->check_over_range(index, N);
			this->index_ += index * (calculat_size<content<Indexs...>>::size);
			return static_cast<xarray_<Element, Package, content<Indexs...>> const&>(*this);
		}
	};

	template<typename Element, typename Package, std::size_t N>
	class xarray_<Element, Package, content<N>>
	{
	public:
		xarray_() :memory_(new Element[calculat_size<Package>::size]), index_(0) {

		}
		xarray_(xarray_&& arr) :memory_(std::move(arr.memory_)), index_(arr.index_)
		{

		}
		virtual ~xarray_() {}
	public:
		Element& operator[](std::size_t index)
		{
			check_over_range(index, N);
			index_ += index;
			auto n = index_;
			index_ = 0;
			return  memory_[n];
		}
		Element const& operator[](std::size_t index) const
		{
			check_over_range(index, N);
			index_ += index;
			auto n = index_;
			index_ = 0;
			return  memory_[n];
		}
	protected:
		void check_over_range(std::size_t i, std::size_t n) const {
			if (i >= n) {
				throw "operator[] over range";
			}
		}
	protected:
		std::unique_ptr<Element[]> memory_;
		mutable std::size_t index_;
	};
	template<typename T>
	struct array_package
	{
		using type = typename traist_array<T, content<>>::type;
	};
	template<typename T>
	using array_package_t = typename array_package<T>::type;
	template<typename T>
	class xarray {};
	template<typename T, std::size_t Size>
	class xarray<T[Size]> :public xarray_<typename array_package_t<T[Size]>::type, typename array_package_t<T[Size]>::params, typename array_package_t<T[Size]>::params>
	{
	public:
		xarray<T[Size]>() = default;
		xarray<T[Size]>(xarray<T[Size]>&& arr) : xarray_<typename array_package_t<T[Size]>::type, typename array_package_t<T[Size]>::params, typename array_package_t<T[Size]>::params>(std::move(arr))
		{

		}
	};
}