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
		template<typename Function, typename Tyy>
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
		field_context(field&& fieldv, F&& callback) :field_(fieldv), callback_(callback) {

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

#define expand_marco(...) __VA_ARGS__
#define args_number()  124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define caculator_argus_count(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,_101,_102,_103,_104,_105,_106,_107,_108,_109,_110,_111,_112,_113,_114,_115,_116,_117,_118,_119,_120,_121,_122,_123,_124,N,...)  N
#define argus_count_(...) expand_marco(caculator_argus_count(__VA_ARGS__))
#ifdef  _MSC_VER 
#define pre_argus(...) unused,__VA_ARGS__
#define argus_count(...) argus_count_(expand_marco(pre_argus(__VA_ARGS__)),args_number())
#else
#define argus_count(...) argus_count_(unused,##__VA_ARGS__,args_number())
#endif

#define concat_a_b(a,b) expand_marco(a##b)
#define vistor_class_0(...) 
#define vistor_class_1(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_0(__VA_ARGS__))
#define vistor_class_2(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_1(__VA_ARGS__))
#define vistor_class_3(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_2(__VA_ARGS__))
#define vistor_class_4(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_3(__VA_ARGS__))
#define vistor_class_5(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_4(__VA_ARGS__))
#define vistor_class_6(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_5(__VA_ARGS__))
#define vistor_class_7(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_6(__VA_ARGS__))
#define vistor_class_8(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_7(__VA_ARGS__))
#define vistor_class_9(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_8(__VA_ARGS__))
#define vistor_class_10(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_9(__VA_ARGS__))
#define vistor_class_11(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_10(__VA_ARGS__))
#define vistor_class_12(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_11(__VA_ARGS__))
#define vistor_class_13(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_12(__VA_ARGS__))
#define vistor_class_14(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_13(__VA_ARGS__))
#define vistor_class_15(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_14(__VA_ARGS__))
#define vistor_class_16(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_15(__VA_ARGS__))
#define vistor_class_17(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_16(__VA_ARGS__))
#define vistor_class_18(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_17(__VA_ARGS__))
#define vistor_class_19(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_18(__VA_ARGS__))
#define vistor_class_20(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_19(__VA_ARGS__))
#define vistor_class_21(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_20(__VA_ARGS__))
#define vistor_class_22(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_21(__VA_ARGS__))
#define vistor_class_23(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_22(__VA_ARGS__))
#define vistor_class_24(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_23(__VA_ARGS__))
#define vistor_class_25(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_24(__VA_ARGS__))
#define vistor_class_26(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_25(__VA_ARGS__))
#define vistor_class_27(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_26(__VA_ARGS__))
#define vistor_class_28(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_27(__VA_ARGS__))
#define vistor_class_29(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_28(__VA_ARGS__))
#define vistor_class_30(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_29(__VA_ARGS__))
#define vistor_class_31(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_30(__VA_ARGS__))
#define vistor_class_32(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_31(__VA_ARGS__))
#define vistor_class_33(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_32(__VA_ARGS__))
#define vistor_class_34(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_33(__VA_ARGS__))
#define vistor_class_35(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_34(__VA_ARGS__))
#define vistor_class_36(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_35(__VA_ARGS__))
#define vistor_class_37(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_36(__VA_ARGS__))
#define vistor_class_38(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_37(__VA_ARGS__))
#define vistor_class_39(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_38(__VA_ARGS__))
#define vistor_class_40(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_39(__VA_ARGS__))
#define vistor_class_41(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_40(__VA_ARGS__))
#define vistor_class_42(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_41(__VA_ARGS__))
#define vistor_class_43(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_42(__VA_ARGS__))
#define vistor_class_44(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_43(__VA_ARGS__))
#define vistor_class_45(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_44(__VA_ARGS__))
#define vistor_class_46(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_45(__VA_ARGS__))
#define vistor_class_47(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_46(__VA_ARGS__))
#define vistor_class_48(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_47(__VA_ARGS__))
#define vistor_class_49(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_48(__VA_ARGS__))
#define vistor_class_50(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_49(__VA_ARGS__))
#define vistor_class_51(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_50(__VA_ARGS__))
#define vistor_class_52(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_51(__VA_ARGS__))
#define vistor_class_53(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_52(__VA_ARGS__))
#define vistor_class_54(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_53(__VA_ARGS__))
#define vistor_class_55(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_54(__VA_ARGS__))
#define vistor_class_56(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_55(__VA_ARGS__))
#define vistor_class_57(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_56(__VA_ARGS__))
#define vistor_class_58(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_57(__VA_ARGS__))
#define vistor_class_59(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_58(__VA_ARGS__))
#define vistor_class_60(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_59(__VA_ARGS__))
#define vistor_class_61(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_60(__VA_ARGS__))
#define vistor_class_62(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_61(__VA_ARGS__))
#define vistor_class_63(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_62(__VA_ARGS__))
#define vistor_class_64(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_63(__VA_ARGS__))
#define vistor_class_65(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_64(__VA_ARGS__))
#define vistor_class_66(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_65(__VA_ARGS__))
#define vistor_class_67(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_66(__VA_ARGS__))
#define vistor_class_68(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_67(__VA_ARGS__))
#define vistor_class_69(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_68(__VA_ARGS__))
#define vistor_class_70(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_69(__VA_ARGS__))
#define vistor_class_71(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_70(__VA_ARGS__))
#define vistor_class_72(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_71(__VA_ARGS__))
#define vistor_class_73(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_72(__VA_ARGS__))
#define vistor_class_74(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_73(__VA_ARGS__))
#define vistor_class_75(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_74(__VA_ARGS__))
#define vistor_class_76(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_75(__VA_ARGS__))
#define vistor_class_77(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_76(__VA_ARGS__))
#define vistor_class_78(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_77(__VA_ARGS__))
#define vistor_class_79(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_78(__VA_ARGS__))
#define vistor_class_80(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_79(__VA_ARGS__))
#define vistor_class_81(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_80(__VA_ARGS__))
#define vistor_class_82(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_81(__VA_ARGS__))
#define vistor_class_83(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_82(__VA_ARGS__))
#define vistor_class_84(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_83(__VA_ARGS__))
#define vistor_class_85(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_84(__VA_ARGS__))
#define vistor_class_86(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_85(__VA_ARGS__))
#define vistor_class_87(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_86(__VA_ARGS__))
#define vistor_class_88(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_87(__VA_ARGS__))
#define vistor_class_89(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_88(__VA_ARGS__))
#define vistor_class_90(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_89(__VA_ARGS__))
#define vistor_class_91(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_90(__VA_ARGS__))
#define vistor_class_92(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_91(__VA_ARGS__))
#define vistor_class_93(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_92(__VA_ARGS__))
#define vistor_class_94(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_93(__VA_ARGS__))
#define vistor_class_95(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_94(__VA_ARGS__))
#define vistor_class_96(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_95(__VA_ARGS__))
#define vistor_class_97(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_96(__VA_ARGS__))
#define vistor_class_98(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_97(__VA_ARGS__))
#define vistor_class_99(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_98(__VA_ARGS__))
#define vistor_class_100(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_99(__VA_ARGS__))
#define vistor_class_101(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_100(__VA_ARGS__))
#define vistor_class_102(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_101(__VA_ARGS__))
#define vistor_class_103(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_102(__VA_ARGS__))
#define vistor_class_104(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_103(__VA_ARGS__))
#define vistor_class_105(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_104(__VA_ARGS__))
#define vistor_class_106(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_105(__VA_ARGS__))
#define vistor_class_107(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_106(__VA_ARGS__))
#define vistor_class_108(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_107(__VA_ARGS__))
#define vistor_class_109(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_108(__VA_ARGS__))
#define vistor_class_110(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_109(__VA_ARGS__))
#define vistor_class_111(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_110(__VA_ARGS__))
#define vistor_class_112(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_111(__VA_ARGS__))
#define vistor_class_113(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_112(__VA_ARGS__))
#define vistor_class_114(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_113(__VA_ARGS__))
#define vistor_class_115(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_114(__VA_ARGS__))
#define vistor_class_116(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_115(__VA_ARGS__))
#define vistor_class_117(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_116(__VA_ARGS__))
#define vistor_class_118(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_117(__VA_ARGS__))
#define vistor_class_119(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_118(__VA_ARGS__))
#define vistor_class_120(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_119(__VA_ARGS__))
#define vistor_class_121(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_120(__VA_ARGS__))
#define vistor_class_122(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_121(__VA_ARGS__))
#define vistor_class_123(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_122(__VA_ARGS__))
#define vistor_class_124(element,...) v & reg_field(element); \
                                      expand_marco(vistor_class_123(__VA_ARGS__))
#define vistor_class_(N,...) \
template<typename T, typename Function,typename U> \
friend xmh::vistor<std::remove_reference_t<Function>> xmh::make_vistor(T& t, Function&& function);\
private: \
template<typename Vistor> \
void vistor(Vistor& v)\
{\
 expand_marco(concat_a_b(vistor_class_,N)(__VA_ARGS__))\
}

#define vistor_class(...)  expand_marco(vistor_class_(argus_count(__VA_ARGS__),__VA_ARGS__))

template<typename T, typename Function, typename U = std::void_t<decltype(&T::template vistor <vistor<std::remove_reference_t<Function>> >) >>
vistor<std::remove_reference_t<Function>> make_vistor(T& t, Function&& function)
{
	vistor<std::remove_reference_t<Function>> v{ std::forward<Function>(function) };
	t.vistor(v);
	return v;
}
}