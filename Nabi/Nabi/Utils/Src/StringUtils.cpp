#include "EngineCore.h"

#include "StringUtils.h"

#include "DebugUtils.h"

namespace nabi::StringUtils
{
	std::vector<std::string_view> SplitString(std::string_view const string, char const after, SplitSettings const& splitSettings) NABI_NOEXCEPT
	{
		size_t const afterPos = string.find(after) + 1u;
		std::string_view const afterString = string.substr(afterPos);

		return SplitString(afterString, splitSettings);
	}

	std::vector<std::string_view> SplitString(std::string_view const string, SplitSettings const& splitSettings) NABI_NOEXCEPT
	{
		std::function<std::string_view(std::string_view&)> trimHelper = [](std::string_view& stringView) -> std::string_view { return stringView; };
		if (splitSettings.m_TrimEachValue)
		{
			trimHelper = [](std::string_view& stringView) -> std::string_view { return stringView = Trim(stringView); };
		}

		std::vector<std::string_view> result;
		result.reserve(std::count(string.begin(), string.end(), splitSettings.m_Delimiter) + 1);

		for (std::string_view::const_iterator end = string.begin();; ++end)
		{
			std::string_view::const_iterator const start = end;
			end = std::find(end, string.end(), splitSettings.m_Delimiter);

			result.emplace_back(start, end);
			trimHelper(result.back());

			if (end == string.end())
			{
				ASSERT_CODE
				(
					if (splitSettings.m_ExpectedValues != SIZE_MAX)
					{
						ASSERT(result.size() == splitSettings.m_ExpectedValues, "Tried to split the string, but there wasn't the expected number of substrings!");
					}
				)
				return result;
			}
		}
	}
} // namespace nabi::StringUtils
