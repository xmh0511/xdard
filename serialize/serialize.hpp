#pragma once
#include "reflector.hpp"
#include "json.hpp"
namespace xmh {
	namespace serializer {
		template<typename T, typename U>
		inline nlohmann::json to_json(T&& obj, nlohmann::json& json);
		template<typename T, typename U>
		inline nlohmann::json to_json(T&& obj);

		template<typename T, typename U>
		inline std::remove_reference_t<T> to_object(T&& t, nlohmann::json const& json);
		template<typename T, typename U>
		inline std::remove_reference_t<T> to_object(nlohmann::json const& json);
	}
	class serialize_ {
		template<typename T, typename U>
		friend nlohmann::json serializer::to_json(T&& obj, nlohmann::json& json);
		template<typename T, typename U>
		friend nlohmann::json serializer::to_json(T&& obj);
		template<typename T, typename U>
		friend std::remove_reference_t<T> serializer::to_object(T&& t, nlohmann::json const& json);
		template<typename T, typename U>
		friend std::remove_reference_t<T> serializer::to_object(nlohmann::json const& json);
	private:
		template<typename T>
		static std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value> to_json_v(T&& obj, std::string const& name, nlohmann::json& json)
		{
			json[name] = nlohmann::json::object();
			to_json(obj, json[name]);
		}

		template<typename T>
		static std::enable_if_t<!reflector::is_reflect_class<std::remove_reference_t<T>>::value> to_json_v(T&& obj, std::string const& name, nlohmann::json& json)
		{
			json[name] = obj;
		}
		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		static void to_json(T&& obj, nlohmann::json& json)
		{
			reflector::each_object(std::forward<T>(obj), [&json](auto& t, auto& name, auto offset) {
				to_json_v((t.*offset), name, json);
			});
		}
		template<typename T, typename U>
		static std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value> to_object_v(T& t, U&& v)
		{
			to_object(t, std::forward<U>(v));
		}

		template<typename T, typename U>
		static std::enable_if_t<!reflector::is_reflect_class<std::remove_reference_t<T>>::value> to_object_v(T& t, U&& v)
		{
			t = v.get<T>();
		}

		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		static void to_object(T&& t, nlohmann::json const& json)
		{
			reflector::each_object(std::forward<T>(t), [&json](auto& t, auto& name, auto offset) {
				to_object_v((t.*offset), json[name]);
			});
		}
   };
	namespace serializer {
		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		inline nlohmann::json to_json(T&& obj, nlohmann::json& json)
		{
			serialize_::to_json(std::forward<T>(obj), json);
			return json;
		}
		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		inline nlohmann::json to_json(T&& obj)
		{
			nlohmann::json root;
			serialize_::to_json(std::forward<T>(obj), root);
			return root;
		}

		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		inline std::remove_reference_t<T> to_object(T&& t, nlohmann::json const& json)
		{
			serialize_::to_object(std::forward<T>(t), json);
			return std::forward<T>(t);
		}

		template<typename T, typename U = std::enable_if_t<reflector::is_reflect_class<std::remove_reference_t<T>>::value>>
		inline std::remove_reference_t<T> to_object(nlohmann::json const& json)
		{
			T t;
			serialize_::to_object(t, json);
			return t;
		}
	}
}