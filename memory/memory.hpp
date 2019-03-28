#pragma once
namespace xmh {
    /* unique_ptr implement begin */
	template<typename T>
	class unique_ptr
	{
	public:
		unique_ptr():raw_pointer_(nullptr)
		{

		}
		unique_ptr(T* ptr) :raw_pointer_(ptr) {}
		unique_ptr(unique_ptr&& uptr):raw_pointer_(uptr.raw_pointer_)
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

	template<typename T,typename...Args>
	unique_ptr<T> make_unique(Args&&...args)
	{
		return unique_ptr<T>(new T{ std::forward<Args>(args)... });
	}
	/* unique_ptr implement end */

	template<typename T>
	class pointer_count
	{
	public:
		pointer_count(T* raw_ptr) :raw_pointer_(raw_ptr)
		{

		}
		T* get()
		{
			return raw_pointer_;
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
					delete raw_pointer_;
					raw_pointer_ = nullptr;
				}
				delete this;
			}
		}
	private:
		T* raw_pointer_ = nullptr;
		unsigned long long ref_counts_ = 1;
	};
	template<typename T>
	class shared_ptr
	{
	public:
		shared_ptr() = default;
		shared_ptr(T* sptr):pointer_ref_(new pointer_count<T>(sptr))
		{
             
		}
		shared_ptr(shared_ptr const& sptr):pointer_ref_(sptr.pointer_ref_)
		{
			pointer_ref_->share();
		}
		shared_ptr& operator=(shared_ptr const& sptr)
		{
			if (pointer_ref_ != nullptr) {
				pointer_ref_->destory();
			}
			pointer_ref_ = sptr.pointer_ref_;
			pointer_ref_->share();
			return *this;
		}
		T* operator ->()
		{
			return pointer_ref_->get();
		}
		T& operator*()
		{
			return *(pointer_ref_->get());
		}
	public:
		~shared_ptr()
		{
			pointer_ref_->destory();
		}
	private:
		pointer_count<T>* pointer_ref_ = nullptr;
	};

	template<typename T,typename...Args>
	shared_ptr<T> make_shared(Args&&...args)
	{
		return shared_ptr<T>(new T{ std::forward<Args>(args)... });
	}
}