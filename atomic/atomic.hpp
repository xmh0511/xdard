#pragma once
namespace xmh {
	template<typename T, typename U = typename std::enable_if<std::is_trivially_constructible<T>::value,int>::type>
	class atomic {
	public:
		atomic(U init) :addr_(new U{}) {
			*addr_ = init;
		}
		atomic() = delete;
		atomic(atomic const&) = delete;
		atomic(atomic&&) = delete;
		atomic& operator=(atomic const& v) = delete;
	public:
		void add_value(U delta) {
			U current = *addr_;
			U change = current + delta;
			while (true) {
				U current0 = cmpxchg(addr_, change, current);
				//std::cout << "before change current= " << current << " return eax  = " << current0 <<"  addr_ = "<<*addr_<< std::endl;
				if (current != current0) {
					current = current0;
					change = current + delta;
				}
				else {
					break;
				}
				//std::cout << "repeat" << std::endl;
			}
		}
		void set_value(U change) {
			U current = *addr_;
			while (true) {
				U current0 = cmpxchg(addr_, change, current);
				if (current != current0) {
					current = current0;
				}
				else {
					break;
				}
			}
		}
		atomic& operator++(int) {
			add_value(1);
			return *this;
		}
		atomic& operator--(int) {
			add_value(-1);
			return *this;
		}
		atomic& operator=(U v) {
			set_value(v);
			return *this;
		}
		atomic& operator+=(U v) {
			add_value(v);
			return *this;
		}
		atomic& operator-=(U v) {
			add_value(-v);
			return *this;
		}
		operator U() {
			return *addr_;
		}
	public:
		friend std::ostream& operator <<(std::ostream& out, atomic const& v) {
			out << *(v.addr_);
			return out;
		}
		~atomic() {
			if (addr_ != nullptr) {
				delete addr_;
				addr_ = nullptr;
			}
		}
	private:
		static int cmpxchg(U* addr__, U change, U current) {
#ifdef  _WIN32
			U* addr = addr__;
			U c = change;
			U current_ = current;
			_asm {
				mov eax, current_
				mov ecx, addr
				mov ebx, c
				lock cmpxchg[ecx], ebx
			}
#else
			asm(
				"movl %edx,%eax\r\n"
				"movq %rdi,%rcx\r\n"
				"movl %esi,%ebx\r\n"
				"lock cmpxchg %ebx,(%rcx)"
			);
#endif //  _Linux
		}
	private:
		U* addr_ = nullptr;
	};
}