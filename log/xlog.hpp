#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <ctime>
#include "filesystem.hpp"
namespace fs = ghc::filesystem;
namespace xmh {
	enum  class log_level :std::uint16_t
	{
		INFO,
		WARNING,
		ERROR0
	};
	class XLog {
		struct path_storage {
			std::string path_;
			std::string file_name;
		};
	public:
		static path_storage& get_storage() {
			static path_storage s;
			return s;
		}
	public:
		static XLog& get(log_level level) {
			static XLog log;
			log.set_level(level);
			return log;
		}
	public:
		static void init(std::string const& root, std::string const& file_name_pre) {
			get_storage().path_ = root;
			get_storage().file_name = file_name_pre;
		}
	private:
		std::string cacl_file_name() {
			auto root = get_storage().path_;
			auto file_name_pre = get_storage().file_name;
			auto time_t = std::time(nullptr);
			char buff[64];
			strftime(buff, 64, "%Y-%m-%d", localtime(&time_t));
			auto file_name = root + "/" + file_name_pre + "_" + std::string(buff) + ".log";
			return file_name;
		}
	private:
		std::string levestr() {
			switch (log_level_)
			{
			case xmh::log_level::INFO:
				return "info";
				break;
			case xmh::log_level::WARNING:
				return "warning";
				break;
			case xmh::log_level::ERROR0:
				return "error";
				break;
			default:
				return "unknow";
				break;
			}
		}
	public:
		XLog(){
			if (!fs::exists(get_storage().path_)) {
				fs::create_directory(get_storage().path_);
			}
		}
	public:
		void set_level(log_level level) {
			std::unique_lock<std::mutex> lock(thread_mutex_);
			log_level_ = level;
		}
	public:
		XLog& operator <<(std::string const& message) {
			std::unique_lock<std::mutex> lock(thread_mutex_);
			file_writer_ = std::make_unique<std::ofstream>(cacl_file_name(), std::ios::app | std::ios::out);
			std::stringstream ss;
			char buff[256];
			auto time_t = std::time(nullptr);
			strftime(buff, 256, "%Y-%m-%d %H:%M:%S", localtime(&time_t));
			ss << "[ " << buff << "   LEVEL:" << levestr() << " ]" << " " << message << "\n";
			(*file_writer_) << ss.str();
			file_writer_->close();
			return *this;
		}
	private:
		std::unique_ptr<std::ofstream> file_writer_;
		std::mutex thread_mutex_;
		log_level log_level_ = log_level::INFO;
	};
}
#define LOGINFO xmh::XLog::get(xmh::log_level::INFO)
#define LOGWARN xmh::XLog::get(xmh::log_level::WARNING)
#define LOGERR xmh::XLog::get(xmh::log_level::ERROR0)