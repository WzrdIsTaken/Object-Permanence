#pragma once
#include "Core.h"

#include "StringConverter.h"
#include "StringUtils.h"

#ifdef RUN_TESTS

namespace nabi::Tests
{
	struct MockCustomDataType final
	{
		double m_DoubleType;
		bool m_BoolType;

		static MockCustomDataType FromString(std::string const& string)
		{
			using namespace nabi;

			StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
			splitSettings.m_ExpectedValues = 2u;
			std::vector<std::string_view> const splitString = StringUtils::SplitString(string, splitSettings);

			double const doubleType = Reflection::StringConverter::FromString<double>(splitString[0].data());
			bool const boolType = Reflection::StringConverter::FromString<bool>(splitString[1].data());

			MockCustomDataType const result = { doubleType, boolType };
			return result;
		}

		bool operator == (MockCustomDataType const other) const
		{
			return this->m_DoubleType == other.m_DoubleType &&
				   this->m_BoolType   == other.m_BoolType;
		}
	};
} // namespace nabi::Tests

#endif // #ifdef RUN_TESTS
