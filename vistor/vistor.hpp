#pragma once
#include <string>
#include <map>
#include <memory>
namespace xmh {

	class field_implement {
	public:
		virtual std::string name() const = 0;
		virtual void invoke() const = 0;
	};
	template<typename Ty>
	class field
	{
		template<typename Function, typename Ty>
		friend class field_context;
		template<typename Function>
		friend class vistor;
	public:
		using type = Ty;
	public:
		template<typename T>
		field(std::string const& name, T& offset) :name_(name), offset_(offset)
		{

		}
	protected:
		std::string name_;
		Ty& offset_;
	};
	template<typename Function, typename Ty>
	class field_context :public field_implement {
	public:
		template<typename field, typename F>
		field_context(field&& field, F&& callback) :field_(field), callback_(callback) {

		}
	public:
		std::string name() const {
			return field_.name_;
		}
		void invoke() const {
			callback_(static_cast<std::string const&>(field_.name_), field_.offset_);
		}
	private:
		field<Ty> field_;
		Function callback_;
	};
	template<typename Function>
	class vistor {
	public:
		template<typename F>
		vistor(F&& f) :function_(std::forward<F>(f)) {

		}
		template<typename field>
		vistor& operator &(field&& f) {
			fields_.emplace(std::make_pair(f.name_, std::make_unique<field_context<Function, typename std::remove_reference_t<field>::type>>(std::forward<field>(f), function_)));
			return *this;
		}
	public:
		void each() {
			for (auto& iter : fields_) {
				iter.second->invoke();
			}
		}
		void find(std::string const& name) {
			auto it = fields_.find(name);
			if (it != fields_.end()) {
				it->second->invoke();
			}
		}
	private:
		std::map<std::string, std::unique_ptr<field_implement>> fields_;
		Function function_;
	};
#define reg_field(x) xmh::field<decltype(x)>{#x,x}
	template<typename T, typename Function>
	auto make_vistor(T& t, Function&& function)
	{
		vistor<std::remove_reference_t<Function>> v{ std::forward<Function>(function) };
		t.vistor(v);
		return v;
	}
}