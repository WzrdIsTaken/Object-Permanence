#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "StringUtils.h"

// Handles the conversion of strings to types, and includes the basic type + container FromString methods
namespace nabi::Reflection::StringConverter
{
	/// <summary>
	/// Converts a string property value to its correct type, and assigns it to the meta property on the passed in meta object.
	/// </summary>
	/// <param name="propertyValue">- The string property value (eg: "1", "true")</param>
	/// <param name="propertyTypeHash">- The type of the property (eg: "bool" <- but a hashed representation)</param>
	/// <param name="metaObject">- The meta object to assign the converted property to</param>
	/// <param name="metaMember">- The meta property on the meta object to set to the  converted property's value</param>
	void ConvertFromString(std::string const& propertyValue, entt::hashed_string const& propertyTypeHash,
						   entt::meta_any& metaObject, entt::meta_data const& metaMember) NABI_NOEXCEPT;
	// Helper function for the above ^ , just pulls out the actual calling the function logic so can be used elsewhere
	entt::meta_any CallFromStringFunction(entt::meta_type const& dataType, std::string const& dataValue, entt::hashed_string const& propertyTypeHash,
		entt::meta_any* const metaObject = nullptr, entt::meta_data const* const metaMember = nullptr) NABI_NOEXCEPT;

	/// <summary>
	/// Extracts the type from a namespace + type. Eg: MyNamespace::MyType -> MyType.
	/// Can also be used for extracting the values of enums. Eg: MyEnum::One -> One.
	/// Or just about anything else which meets this :: criteria I guess!
	/// </summary>
	/// <param name="typeInfoName">- The full type name</param>
	[[nodiscard]] std::string ExtractTypeName(std::string_view const typeInfoName) NABI_NOEXCEPT;
	void ExtractTypeName(std::string& typeInfoName) NABI_NOEXCEPT;

	/// <summary>
	/// Checks if a value could be an enum and if so converts it to an the underlying enum's type of T.
	/// Why only underlying types? Because in the only situations we need to call this code (see FromString<uint>)
	/// we need the underlying type. The way we check if something could be an enum is if it has the "::" characters.
	/// Because the type the reflected value is being assigned to is an underlying type, not the enum itself,
	/// we need the full enum name (unlike other enum from strings) to allow us to convert it. 
	/// </summary>
	/// <typeparam name="T">- The underlying type of the enum</typeparam>
	/// <param name="string">- The string version of the enum. Note it must be the full enum name</param>
	/// <param name="value">- Will be set to a value if the conversion to enum was successful</param>
	/// <returns>If the conversion was successful</returns>
	template<typename T>
	bool CheckIfValueCouldBeEnum(std::string const& string, T* const value) NABI_NOEXCEPT
	{
		if (string.find("::") != std::string::npos)
		{
			size_t const firstColon     = string.find_first_of(':');
			size_t const secondColon    = firstColon + 1u;
			size_t const firstCharacter = string.find_first_not_of(' ');
			size_t const lastCharacter  = string.find_last_not_of(' ');

			std::string const enumName = string.substr(firstCharacter, firstColon);
			std::string const enumValue = string.substr(secondColon + 1u, lastCharacter);

			entt::hashed_string const enumNameHashed = entt::hashed_string(enumName.c_str());
			entt::meta_type const enumType = entt::resolve(enumNameHashed);
			entt::meta_any const result = CallFromStringFunction(enumType, enumValue, enumNameHashed);

			if (result && value)
			{
				*value = result.cast<T>();
			}
			return true;
		}

		return false;
	}

	/// <summary>
	/// This specialised template functions convert a string to a type. They are used to support basic type reflection.
	/// </summary>
	/// <typeparam name="T">- The basic type</typeparam>
	/// <param name="string">- The string to convert to T</param>
	/// <returns>The converted type</returns>
	template<typename T>
	[[nodiscard]] static T FromString(std::string const& string) NABI_NOEXCEPT {};

	// char
	template<>
	[[nodiscard]] char FromString<char>(std::string const& string) NABI_NOEXCEPT
	{
		char const result = string.front();
		return result;
	}

	// int
	template<>
	[[nodiscard]] int FromString<int>(std::string const& string) NABI_NOEXCEPT
	{
		int const result = std::stoi(string);
		return result;
	}

	// uint
	template<>
	[[nodiscard]] unsigned int FromString<unsigned int>(std::string const& string) NABI_NOEXCEPT
	{
		/*
		  Ok this is super jank. So - what happens if we make a type engine side which all enums game side will be, for example
		  "AudioID" (AudioStateComponent.h) or "CollisionID" (ColliderComponent.h). Well, in the reflection code which uses it we
		  will be dealing with the underlying type, not the enum. Therefore, when ConvertFromString is called it won't be able to 
		  go from the string enum name, to the underlying enum type. 

		  My (bad!) solution for this is to use this CheckIfValueCouldBeEnum function in all base type FromString functions which
		  I might make an enum a type of. This code can't go in ConvertFromString because if eg the underlying enum type is part of
		  a container, then ConvertFromString will be called for the container but not the underlying type, only FromString
		  will be called for the underlying type.

		  So, if a type of unsigned int's string value contains the characters "::", then we check if it could be an enum. If it is,
		  then we use that value instead of the direct string -> unsigned int conversion. 

		  Yes, this is kinda bot.
		*/

		unsigned int result;
		if (!CheckIfValueCouldBeEnum(string, &result))
		{
			result = std::stoul(string);
		}

		return result;
	}

	// ulonglong
	template<>
	[[nodiscard]] unsigned long long FromString<unsigned long long>(std::string const& string) NABI_NOEXCEPT
	{
		unsigned long long result;
		if (!CheckIfValueCouldBeEnum(string, &result))
		{
			result = std::stoull(string);
		}

		return result;
	}

	// float
	template<>
	[[nodiscard]] float FromString<float>(std::string const& string) NABI_NOEXCEPT
	{
		float const result = std::stof(string);
		return result;
	}

	// double
	template<>
	[[nodiscard]] double FromString<double>(std::string const& string) NABI_NOEXCEPT
	{
		double const result = std::stod(string);
		return result;
	}

	// bool
	template<>
	[[nodiscard]] bool FromString<bool>(std::string const& string) NABI_NOEXCEPT
	{
		/* cool but bot
			std::string& inputString = const_cast<std::string&>(string); // unfortunate
			std::transform(inputString.begin(), inputString.end(), inputString.begin(), std::tolower);

			std::stringstream ss(inputString);

			bool result;
			ss >> std::boolalpha >> result;

			return result;
		*/

		bool result = false;
		
		if (string == "true" || string == "True" || string == "TRUE" || string == "1")
		{
			result = true;
		}

		return result;
	}

	// string
	template<>
	[[nodiscard]] std::string FromString<std::string>(std::string const& string) NABI_NOEXCEPT
	{
		std::string const result = string;
		return result;
	}

	/// <summary>
	/// The functions below can be used to convert std containers from a string.
	/// Container values are separated by commas (,) and entries by the pipe (|)
	/// For example, for a map data could look like "x, y | a, b"
	/// 
	/// If you get an error like "nabi::Reflection::StringConverter::FromString<nabi::Tests::MyCustomDataType>': must return a value" it probably
	/// means that you tried to use a custom type in a container. This is not currently supported. 
	/// Consider using multiple containers and indexing into them. Eg:
	/// vec1 { 1, 2, 3 }
	/// vec2 { 4.0, 5.0, 6.0 }
	/// auto customType = { vec1[0], vec2[1] } 
	/// This is definitely going to be faster than implementing custom type support for the (possible) few cases where reflected custom types in containers are needed.
	/// Reflected a nested custom type (eg, struct MyTypeOne { struct MyTypeTwo // reflect this } don't work for the same reason. 
	/// tl;dr, FromString is called to turn the string into a type, but it only is templated for primitive types.
	/// A solution to this could be making the all custom type FromString methods a template specialization, eg in MyTypeTwo define a method FromString<MyTypeTwo>
	/// This would allow the base FromString method call to 'find' custom types. Tbh this doesn't actually sound too bad of a refactor... 
	/// I didn't think of doing this (obv..) when I was writing the reflection code for the first time. I'll make a note.
	/// </summary>
	
	namespace Containers
	{
		char constexpr c_EntryDelimiter = '|';
		char constexpr c_ValueDelimiter = ',';
	} // namespace Containers

	template<typename Key, typename Value>
	[[nodiscard]] std::map<Key, Value> StdMapFromString(std::string const& string) NABI_NOEXCEPT
	{
		std::map<Key, Value> result = {};

		// Split the string into key value pairs
		nabi::StringUtils::SplitSettings splitSettings = nabi::StringUtils::c_DefaultSplitSettings;
		splitSettings.m_Delimiter = Containers::c_EntryDelimiter;
		std::vector<std::string_view> const keyValuePairs = nabi::StringUtils::SplitString(string, splitSettings);

		// Loop through the entries and split them
		for (std::string_view const keyValuePair : keyValuePairs)
		{
			// Split the key value pairs into the key and value
			splitSettings.m_Delimiter = Containers::c_ValueDelimiter;
			splitSettings.m_ExpectedValues = 2u;
			std::vector<std::string_view> const splitKeyValuePair = nabi::StringUtils::SplitString(keyValuePair, splitSettings);

			// Add the key / value to the map
			std::string_view const key   = splitKeyValuePair.at(0u);
			std::string_view const value = splitKeyValuePair.at(1u);

			result.emplace(
				nabi::Reflection::StringConverter::FromString<Key  >(std::string(key.data(),   key.length())),
				nabi::Reflection::StringConverter::FromString<Value>(std::string(value.data(), value.length()))
			);
		}

		return result;
	}

	template<typename T>
	[[nodiscard]] std::vector<T> StdVectorFromString(std::string const& string) NABI_NOEXCEPT
	{
		std::vector<T> result = {};

		nabi::StringUtils::SplitSettings splitSettings = nabi::StringUtils::c_DefaultSplitSettings;
		splitSettings.m_Delimiter = Containers::c_ValueDelimiter;
		std::vector<std::string_view> const values = nabi::StringUtils::SplitString(string, splitSettings);

		for (std::string_view const value : values)
		{
			result.emplace_back(nabi::Reflection::StringConverter::FromString<T>(std::string(value.data(), value.length())));
		}

		return result;
	}

	/// <summary>
	/// Converts the type T to a reflection friendly string format.
	/// Eg, a custom type would be converted to value1, value2, value3 etc
	/// 
	/// I'm going to try my custom type template specialisation idea with this
	/// function. If it works out / I have time - I'll convert FromString to 
	/// do the same. Fingers crossed! I'll add these overrides as I need them.
	/// </summary>
	template<typename T>
	[[nodiscard]] static std::string ToString(T const& type) NABI_NOEXCEPT { return "yo~!"; };

	template<>
	[[nodiscard]] std::string ToString<float>(float const& type) NABI_NOEXCEPT
	{
		return std::to_string(type);
	}
} // namespace nabi::Reflection::StringConverter

/*
* Could use before CheckIfValueCouldBeEnum. Will keep these around in case I need a quick copy paste
namespace entt
{
	class meta_any;
	class meta_type;
	struct meta_data;

	template<typename Char>
	class basic_hashed_string;
	using hashed_string = basic_hashed_string<char>;
}
*/
