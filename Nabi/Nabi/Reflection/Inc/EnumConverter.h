#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "DebugUtils.h"
#include "StringUtils.h"

namespace nabi::Reflection::EnumConverter
{
	/// <summary>
	/// Turns a string enum name to its actual enum value. Just use the name, eg: "MyEnum::Value" X | "Value" O
	/// Also works with enum flags! Pass in the enum names, with an |. Eg: "One|Two"
	/// </summary>
	/// <typeparam name="T">The enum</typeparam>
	/// <param name="enumName">- The name of the enum value</param>
	/// <returns>The enum value</returns>
	template<typename T>
	[[nodiscard]] T StringToEnum(std::string const& enumName) NABI_NOEXCEPT
	{
		/*
			Why std::string const& not std::string_view ? Because const& is needed for the deserialization xml->object stuff to work.
			And having two functions one with string_view and one with a normal string makes entt confused...
			Tbh thats just given me a thought - would rewriting the reflection ToString stuff to use string_view fix the string parsing issue 
			(see StringConverter::ConvertFromString (cpp)) as now the functions first arg wouldn't be a std::string? maybe... 
			But the reflection works and I don't want to touch it right now :D I'm like 50% sure past Ben already tried it as well...

			The reason why we need to use enumBitwiseOrHelper and not just or the enum directly is because not all enums have the 
			bitwise operators defined. Therefore, the template wouldn't compile.
		*/

		auto stringToEnumHelper =
			[](std::string const& name) -> T
			{
				entt::hashed_string const enumNameHash = entt::hashed_string(name.c_str());
				return entt::resolve<T>().data(enumNameHash).get({}).cast<T>();
			};

		auto enumBitwiseOrHelper =
			[](T const lhs, T const rhs) -> T
			{
				typedef std::underlying_type_t<T> EnumType;
				return static_cast<T>(static_cast<EnumType>(lhs) | static_cast<EnumType>(rhs));
			};

		char constexpr bitwiseOr = '|';
		if (enumName.find(bitwiseOr) != std::string::npos)
		{
			// Or the expression

			using namespace nabi;

			StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
			splitSettings.m_Delimiter = bitwiseOr;

			std::vector<std::string_view> enums = StringUtils::SplitString(enumName, splitSettings);
			size_t const enumCount = enums.size();
			T enumValue = stringToEnumHelper(StringUtils::StringFromStringView(enums.front()));

			if (enumCount > 1)
			{
				for (size_t i = 1u; i < enumCount; ++i)
				{
					T const enumValueToOr = stringToEnumHelper(StringUtils::StringFromStringView(enums[i]));
					enumValue = enumBitwiseOrHelper(enumValue, enumValueToOr);
				}
			}
			else
			{
				ASSERT_FAIL("Using bitwise or with an enum that only has one value. Is this a typo?");
			}

			return enumValue;
		}
		else
		{
			// Convert the enum like normal

			T const enumValue = stringToEnumHelper(enumName);
			return enumValue;
		}
	}

	template<typename T>
	[[nodiscard]] std::underlying_type_t<T> StringToEnumUnderlyingValue(std::string const& enumName) NABI_NOEXCEPT
	{
		typedef std::underlying_type_t<T> EnumType;

		T const enumValue = StringToEnum<T>(enumName);
		EnumType const enumUnderlyingValue = static_cast<EnumType>(enumValue);

		return enumUnderlyingValue;
	}

	// Soon tm!
	[[nodiscard]] inline std::string EnumToString(int const /*_enum*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED

		// Todo - Implement me! (If its ever needed...)
		return {};
	}
} // namespace nabi::Reflection::EnumConverter
