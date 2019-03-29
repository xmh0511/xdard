#pragma once
#include <atomic>
namespace xmh {
	/* unique_ptr implement begin */
	template<typename T>
	class unique_ptr
	{
	public:
		unique_ptr() :raw_pointer_(nullptr)
		{

		}
		unique_ptr(T* ptr) :raw_pointer_(ptr) {}
		unique_ptr(unique_ptr&& uptr) :raw_pointer_(uptr.raw_pointer_)
		{
			uptr.raw_pointer_ = nullptr;
		}
		unique_ptr& operator=(unique_ptr&& uptr)
		{
			raw_pointer_ = uptr.raw_pointer_;
			uptr.raw_pointer_ = nullptr;
			return *this;
		}
		T* get()
		{
			return raw_pointer_;
		}
	public:
		T* operator ->()
		{
			return raw_pointer_;
		}

		T& operator*()
		{
			return *raw_pointer_;
		}
	public:
		~unique_ptr()
		{
			if (raw_pointer_ != nullptr)
			{
				delete raw_pointer_;
				raw_pointer_ = nullptr;
			}
		}
	private:
		unique_ptr(unique_ptr const&) = delete;
		unique_ptr& operator=(unique_ptr const&) = delete;
	private:
		T* raw_pointer_;
	};

	template<typename T, typename...Args>
	unique_ptr<T> make_unique(Args&&...args)
	{
		return unique_ptr<T>(new T{ std::forward<Args>(args)... });
	}
	/* unique_ptr implement end */

	/* shared_ptr implement start */

	class pointer_count_implement
	{
	public:
		virtual ~pointer_count_implement() {}
	public:
		virtual void destory() = 0;
		virtual void share() = 0;
	protected:
		//unsigned long long ref_counts_ = 1;
		std::atomic<std::size_t> ref_counts_ = 1;//thread safe;
	};

	template<typename T, typename Deleter>
	class pointer_count :public pointer_count_implement
	{
	public:
		template<typename Callable>
		pointer_count(T* raw_ptr, Callable&& deleter) :deleter_(std::forward<Callable>(deleter)), raw_pointer_(raw_ptr)
		{

		}
		void share()
		{
			++ref_counts_;
		}
	public:
		void destory()
		{
			if ((--ref_counts_) == 0) {
				if (raw_pointer_ != nullptr) {
					//delete raw_pointer_;
					deleter_(raw_pointer_);
					raw_pointer_ = nullptr;
				}
				delete this;
			}
		}
	private:
		Deleter deleter_;
		T* raw_pointer_ = nullptr;
	};

	template<typename Type>
	class resource_manager
	{
		template<typename Class>
		friend class resource_manager;
	public:
		resource_manager() = default;
		resource_manager(Type* ptr) :raw_pointer_(ptr)
		{
			auto default_deleter = [](auto ptr) {
				delete ptr;
			};
			ref_counter_ = new pointer_count<Type, std::remove_reference_t<decltype(default_deleter)>>(ptr, default_deleter);
		}
		template<typename Callable>
		resource_manager(Type* ptr, Callable&& deleter) :raw_pointer_(ptr)
		{
			ref_counter_ = new pointer_count<Type, std::remove_reference_t<Callable>>(ptr, std::forward<Callable>(deleter));
		}
		resource_manager(resource_manager const& rm) :raw_pointer_(rm.raw_pointer_), ref_counter_(rm.ref_counter_)
		{
			ref_counter_->share();
		}
		template<typename Derived, typename U = std::enable_if_t<(std::is_base_of_v<Type, Derived> || std::is_same_v<Type, Derived>)>>
		resource_manager(resource_manager<Derived> const& rm) : raw_pointer_(rm.raw_pointer_), ref_counter_(rm.ref_counter_)
		{
			ref_counter_->share();
		}
		resource_manager& operator = (resource_manager const& rm)
		{
			if (ref_counter_ != nullptr) {
				ref_counter_->destory();
			}
			raw_pointer_ = rm.raw_pointer_;
			ref_counter_ = rm.ref_counter_;
			ref_counter_->share();
			return *this;
		}
		template<typename Derived, typename U = std::enable_if_t<(std::is_base_of_v<Type, Derived> || std::is_same_v<Type, Derived>)>>
		resource_manager& operator=(resource_manager<Derived> const& rm)
		{
			if (ref_counter_ != nullptr) {
				ref_counter_->destory();
			}
			raw_pointer_ = rm.raw_pointer_;
			ref_counter_ = rm.ref_counter_;
			ref_counter_->share();
			return *this;
		}
	public:
		operator bool()
		{
			if (ref_counter_ == nullptr) {
				return false;
			}
			return true;
		}
	public:
		Type* get() {
			return raw_pointer_;
		}
	public:
		~resource_manager() {
			if (ref_counter_ != nullptr)
			ref_counter_->destory();
		}
	private:
		Type* raw_pointer_ = nullptr;
		pointer_count_implement* ref_counter_ = nullptr;
	};
	template<typename T>
	class shared_ptr;
	template<typename T>
	class shared_ptr
	{
		template<typename Class>
		friend class shared_ptr;
	public:
		shared_ptr() = default;

		template<typename Type>
		shared_ptr(Type* sptr) :pointer_ref_(sptr)
		{
		}
		shared_ptr(shared_ptr const& sptr) :pointer_ref_(sptr.pointer_ref_)
		{
			/*pointer_ref_->share();*/
		}
		template<typename Derived>
		shared_ptr(shared_ptr<Derived> const& sptr) : pointer_ref_(sptr.pointer_ref_)
		{

		}

		shared_ptr& operator=(shared_ptr const& sptr)
		{
			pointer_ref_ = sptr.pointer_ref_;
			return *this;
		}
		template<typename Derived>
		shared_ptr& operator=(shared_ptr<Derived> const& sptr)
		{
			pointer_ref_ = sptr.pointer_ref_;
			return *this;
		}

		T* operator ->()
		{
			return pointer_ref_.get();
		}
		T& operator*()
		{
			return *(pointer_ref_.get());
		}
	public:
		operator bool()
		{
			return bool(pointer_ref_);
		}
	public:
		~shared_ptr()
		{
			/*pointer_ref_->destory();*/
		}
	private:
		resource_manager<T> pointer_ref_;
	};

	template<typename T, typename...Args>
	shared_ptr<T> make_shared(Args&&...args)
	{
		return shared_ptr<T>(new T{ std::forward<Args>(args)... });
	}
	/* shared_ptr implement end */
}