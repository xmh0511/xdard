#pragma once
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <mutex>
namespace xmh {
	template<typename Type>
	class Ref {
	public:
		Ref() = default;
	public:
		template<typename T, typename U = std::enable_if_t<!std::is_same_v<T, Ref>>>
		Ref(T& t) :data_(&t) {

		}
		Type& get() {
			return *data_;
		}
	private:
		Type* data_;
	};
	class Observer;
	class subscribe_base {
	public:
		virtual ~subscribe_base() {}
	};
	template<typename Type>
	class subscriber :public subscribe_base {
		friend class Observer;
	public:
		template<typename Function>
		subscriber(Function&& callback) :call_back_(std::move(callback)) {

		}
	public:
		void dispath() {
			call_back_(data_);
		}
	private:
		Type data_;
		std::function<void(Type)> call_back_;
	};

	class Observer {
	public:
		template<typename Type, typename Function>
		void subscribe(std::string const& event, Function&& callback) {
			event_pool_.emplace(event, std::make_unique<subscriber<Type>>(std::move(callback)));
		}
		template<typename Type>
		void dispatch(std::string const& event, Type&& data) {
			std::unique_lock<std::mutex> lock(mutex_);
			using type = std::remove_reference_t<Type>;
			auto iter = event_pool_.find(event);
			while (iter != std::end(event_pool_)) {
				auto baseptr = &(*(iter->second));
				auto subptr = dynamic_cast<subscriber<type>*>(baseptr);
				if (subptr != nullptr) {
					subptr->data_ = std::move(data);
					subptr->dispath();
				}
				++iter;
			}
		}
	private:
		std::multimap<std::string, std::unique_ptr<subscribe_base>> event_pool_;
		std::mutex mutex_;
	};

	template<typename T>
	Ref<std::remove_reference_t<T>> ref(T& t) {
		return Ref<std::remove_reference_t<T>>(t);
	}
}