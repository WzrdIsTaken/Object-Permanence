#include "EngineCore.h"

#include "BaseTypeReflection.h"

#include "ReflectionIncludes.h"

#include "StringStore.h"

namespace nabi::Reflection
{
	BaseTypeReflection::BaseTypeReflector BaseTypeReflection::s_BaseTypeReflectorInstance;

	BaseTypeReflection::BaseTypeReflector::BaseTypeReflector() NABI_NOEXCEPT
	{
		ReflectPrimitiveTypes();
		// might reflect other things in the future
	}

	void BaseTypeReflection::BaseTypeReflector::ReflectPrimitiveTypes() const NABI_NOEXCEPT
	{
		REFLECT_PRIMITIVE_TYPE(char)
		REFLECT_PRIMITIVE_TYPE(int);
		REFLECT_PRIMITIVE_TYPE(unsigned int);
		REFLECT_PRIMITIVE_TYPE_CUSTOM(unsigned long long, "unsigned __int64");
		REFLECT_PRIMITIVE_TYPE(float);
		REFLECT_PRIMITIVE_TYPE(double);
		REFLECT_PRIMITIVE_TYPE(bool);
		REFLECT_PRIMITIVE_TYPE(std::string);
	}
} // namespace nabi::Reflection

namespace nabi::Reflection::DirectXTypes
{
	// Nothing fancy here, just best to be explicit I think
	
	using namespace dx; // this using namespace is kinda sneaky, cos it means that the directx type name can be stringified directly 

	REFLECT_DATA_TYPE_CUSTOM_DEFAULT(XMFLOAT2, Float2::FromString)
	REFLECT_DATA_TYPE_CUSTOM_DEFAULT(XMFLOAT3, Float3::FromString)
	REFLECT_DATA_TYPE_CUSTOM_DEFAULT(XMINT2, Int2::FromString)
	REFLECT_DATA_TYPE_CUSTOM_DEFAULT(XMINT3, Int3::FromString)

	XMFLOAT2 Float2::FromString(std::string const& source) NABI_NOEXCEPT
	{
		StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 2u;
		std::vector<std::string_view> const splitString = StringUtils::SplitString(source, splitSettings);

		FLOAT const float1 = StringConverter::FromString<FLOAT>(splitString[0].data());
		FLOAT const float2 = StringConverter::FromString<FLOAT>(splitString[1].data());

		XMFLOAT2 const result = { float1, float2 };
		return result;
	}

	XMFLOAT3 Float3::FromString(std::string const& source) NABI_NOEXCEPT
	{
		StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 3u;
		std::vector<std::string_view> const splitString = StringUtils::SplitString(source, splitSettings);

		FLOAT const float1 = StringConverter::FromString<FLOAT>(splitString[0].data());
		FLOAT const float2 = StringConverter::FromString<FLOAT>(splitString[1].data());
		FLOAT const float3 = StringConverter::FromString<FLOAT>(splitString[2].data());

		XMFLOAT3 const result = { float1, float2, float3 };
		return result;
	}

	XMINT2 Int2::FromString(std::string const& source) NABI_NOEXCEPT
	{
		StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 2u;
		std::vector<std::string_view> const splitString = StringUtils::SplitString(source, splitSettings);

		INT const int1 = StringConverter::FromString<INT>(splitString[0].data());
		INT const int2 = StringConverter::FromString<INT>(splitString[1].data());

		XMINT2 const result = { int1, int2 };
		return result;
	}

	XMINT3 Int3::FromString(std::string const& source) NABI_NOEXCEPT
	{
		StringUtils::SplitSettings splitSettings = StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 3u;
		std::vector<std::string_view> const splitString = StringUtils::SplitString(source, splitSettings);

		INT const int1 = StringConverter::FromString<INT>(splitString[0].data());
		INT const int2 = StringConverter::FromString<INT>(splitString[1].data());
		INT const int3 = StringConverter::FromString<INT>(splitString[2].data());

		XMINT3 const result = { int1, int2, int3 };
		return result;
	}
} // namespace nabi::Reflection::DirectXTypes

namespace nabi::Reflection::EnttTypes
{
	using namespace entt;

	REFLECT_DATA_TYPE_CUSTOM(hashed_string, "basic_hashed_string<char>", HashedString::FromString)

	namespace HashedString
	{
		hashed_string FromString(std::string const& source) NABI_NOEXCEPT
		{
			std::string const& storedSourceStr = StringStore::Instance()->Add(source);
			hashed_string const result = hashed_string(storedSourceStr.c_str());
			return result;
		}
	} // namespace HashedString
} // namespace nabi::Reflection::EnttTypes
