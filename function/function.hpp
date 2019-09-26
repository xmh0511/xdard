#pragma once
#include <type_traits>
#include <memory>

namespace xmh {
	template<typename Ret, typename...Params>
	class function_storage_implement {
	public:
		virtual Ret invoke(Params...) = 0;
	};

	template<typename Type, typename Function>
	class function_storage;

	template<typename Function, typename Ret, typename...Params>
	class function_storage<Ret(Params...), Function> :public function_storage_implement<Ret, Params...> {
	public:
		template<typename Callable>
		function_storage(Callable&& callable) :function_(std::forward<Callable>(callable)) {

		}
	public:
		Ret invoke(Params...args) {
			return function_(std::forward<Params>(args)...);
		}
	private:
		Function function_;
	};

	template<typename T>
	class function;

	template<typename Ret, typename...Params>
	class function<Ret(Params...)> {
	public:
		template<typename Function>
		function(Function&& function) :callable_storage_(std::make_shared<function_storage<Ret(Params...), Function>>(std::forward<Function>(function))) {

		}
	public:
		template<typename...Args>
		Ret operator()(Args&& ...args) {
			return callable_storage_->invoke(std::forward<Args>(args)...);
		}
	private:
		std::shared_ptr<function_storage_implement<Ret, Params...>> callable_storage_;
	};
}
