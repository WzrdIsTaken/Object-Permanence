#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "DirectXIncludes.h"

#include "entt.h"

#include "StringConverter.h"

namespace nabi::Reflection
{
	/// <summary>
	/// We need to reflect our base types
	/// </summary>
	class BaseTypeReflection final
	{
		friend class BaseTypeReflector;

		class BaseTypeReflector final
		{
		public:
			/// <summary>
			/// A static constructor to reflect all base types
			/// </summary>
			BaseTypeReflector() NABI_NOEXCEPT;

		private:
			void ReflectPrimitiveTypes() const NABI_NOEXCEPT;
		};

		static BaseTypeReflector s_BaseTypeReflectorInstance;
	};
} // namespace nabi::Reflection

namespace nabi::Reflection::DirectXTypes
{
	// All the direct x types i've used in the authoring data need their own reflection function
	// They need to be in namespaces like this because all from string functions must have the name 'FromString'

	namespace Float2
	{
		dx::XMFLOAT2 FromString(std::string const& source) NABI_NOEXCEPT;
	} // namespace Float2
	namespace Float3
	{
		dx::XMFLOAT3 FromString(std::string const& source) NABI_NOEXCEPT;
	} // namespace Float3
	namespace Int2
	{
		dx::XMINT2 FromString(std::string const& source) NABI_NOEXCEPT;
	} // namespace Int2
	namespace Int3
	{
		dx::XMINT3 FromString(std::string const& source) NABI_NOEXCEPT;
	} // namespace Int3
} // namespace nabi::Reflection::DirectXTypes

namespace nabi::Reflection::EnttTypes
{
	namespace HashedString
	{
		entt::hashed_string FromString(std::string const& source) NABI_NOEXCEPT;
	} // namespace HashedString
} // namespace nabi::Reflection::EnttTypes

// ToString functions (must be in the StringConverter namespace)
namespace nabi::Reflection::StringConverter
{
	template<>
	[[nodiscard]] std::string ToString<dx::XMFLOAT3>(dx::XMFLOAT3 const& type) NABI_NOEXCEPT
	{
		return ToString<float>(type.x) + "," + ToString<float>(type.y) + "," + ToString<float>(type.z);
	}
} // namespace nabi::Reflection::StringConverter

