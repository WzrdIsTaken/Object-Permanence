#pragma once
#include "EngineCore.h"

#include <string>
#include <vector>

namespace nabi::StringUtils
{
	// Can't have a function for this which allocates on the stack cos otherwise the returned string ptr will be invalid 
#define STRING_TO_LPCWSTR(string, lpcwstr) \
	std::wstring const wtemp = std::wstring(string.begin(), string.end()); \
    lpcwstr = wtemp.c_str();

#define STRING_TO_LPCWSTR_INLINE(string) \
	std::wstring(string.begin(), string.end()).c_str()

	// A LPCSTR is just a typedef'ed WCHAR
#define STRING_TO_WCHAR(string, wchar) STRING_TO_LPCWSTR(string, wchar)

#define STRING_TO_WCHAR_INLINE(string) STRING_TO_LPCWSTR_INLINE(string)

	struct SplitSettings final
	{
		char m_Delimiter;
		bool m_TrimEachValue;
		size_t m_ExpectedValues;
	};
	SplitSettings constexpr c_DefaultSplitSettings = {
		.m_Delimiter = ',',
		.m_TrimEachValue = true,
		.m_ExpectedValues = SIZE_MAX
	};

	// Splits a string via a delimiter, after a character
	[[nodiscard]] std::vector<std::string_view> SplitString(std::string_view const string, char const after, SplitSettings const& splitSettings) NABI_NOEXCEPT;
	// Splits a string via a delimiter. Fatal assert if expectedValues is not equal to the resultant split
	[[nodiscard]] std::vector<std::string_view> SplitString(std::string_view const string, SplitSettings const& splitSettings) NABI_NOEXCEPT;

	// Constructs a string from a string view
	[[nodiscard]] inline std::string StringFromStringView(std::string_view const string_view) NABI_NOEXCEPT
	{
		return std::string(string_view.data(), string_view.length());
	}

	// Trims a string
    static inline char const* const c_WhiteSpace = " \t\n\r\f\v";

	inline std::string& RightTrim(std::string& string, char const* const trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		string.erase(string.find_last_not_of(trim) + 1);
		return string;
	}
	inline std::string& LeftTrim(std::string& string, char const* const trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		string.erase(0, string.find_first_not_of(trim));
		return string;
	}
	inline std::string& Trim(std::string& string, char const* const trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		return LeftTrim(RightTrim(string, trim), trim);
	}
	inline std::string_view Trim(std::string_view const stringView, char const* const trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		size_t const first = stringView.find_first_not_of(c_WhiteSpace);
		size_t const last = stringView.find_last_not_of(c_WhiteSpace);

		if (first == std::string_view::npos || last == std::string_view::npos)
		{
			return {};
		}

		return stringView.substr(first, last - first + 1);
	}
} // namespace nabi::StringUtils
