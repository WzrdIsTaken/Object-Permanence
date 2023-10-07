#include "EngineCore.h"

#include "WinIncludes.h"

#include "Containers\UVs.h"

#include "ReflectionIncludes.h"

namespace nabi::Rendering
{
	REFLECT_DATA_TYPE_DEFAULT(UVs)
	
	UVs UVs::FromString(std::string const& string) NABI_NOEXCEPT
	{
		nabi::StringUtils::SplitSettings splitSettings = nabi::StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 4u;
		std::vector<std::string_view> const splitString = nabi::StringUtils::SplitString(string, splitSettings);

		float const u1 = nabi::Reflection::StringConverter::FromString<float>(splitString[0].data());
		float const u2 = nabi::Reflection::StringConverter::FromString<float>(splitString[1].data());
		float const v1 = nabi::Reflection::StringConverter::FromString<float>(splitString[2].data());
		float const v2 = nabi::Reflection::StringConverter::FromString<float>(splitString[3].data());

		UVs const result = { u1, u2, v1, v2 };
		return result;
	}
} // namespace nabi::Rendering
