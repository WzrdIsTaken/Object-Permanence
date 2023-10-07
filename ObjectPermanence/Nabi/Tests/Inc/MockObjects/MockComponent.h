#pragma once
#include "Core.h"

#include "DirectXIncludes.h"
#include "ReflectionIncludes.h"

#include "ComponentBase.h"
#include "MockCustomDataType.h"
#include "MockEnum.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ECS
{
	struct MockComponent final : public nabi::ECS::ComponentBase
	{
		MockComponent()
			: m_IntType(5)
			, m_FloatType(20.6f)
			, m_StringType("I wrote this on 20 / 11 / 2022")
			, m_CustomType({ 6.4, true })
			, m_Enum(MockEnum::Ben)
		{
		}

		int m_IntType;
		float m_FloatType;
		std::string m_StringType;
		MockCustomDataType m_CustomType;
		MockEnum m_Enum;
	};

	struct MockComponentWithDirectXTypes final : public nabi::ECS::ComponentBase
	{
		MockComponentWithDirectXTypes()
			: m_Float2(0.0f, 1.0f)
			, m_Float3(2.0f, 3.0f, 4.0f)
			, m_Int2(5, 6)
			, m_Int3(7, 8, 9)
		{
		}

		dx::XMFLOAT2 m_Float2;
		dx::XMFLOAT3 m_Float3;
		dx::XMINT2 m_Int2;
		dx::XMINT3 m_Int3;
	};

	struct MockComponentWithContainers final : public nabi::ECS::ComponentBase
	{
		MockComponentWithContainers()
			: m_IntVector{{ 0, 2, 3 }} // Refected containers have to be defaulted like this, because the underlying container is wrapped in another struct
			, m_StringVector{{"Hello!"}}
			, m_IntMap{{{1, 2}, {3, 4}}}
			//, m_CustomTypeVector{ {{0.1, true}, {0.2, false}} }
		{
		}

		REFLECTED_VECTOR(m_IntVector, int);
		REFLECTED_VECTOR(m_StringVector, std::string);
		REFLECTED_MAP(m_IntMap, int, int);

		// Containers with custom types are not currently supported. See the comment in StringConverter.h to see how they could be.
		//REFLECTED_VECTOR(m_CustomTypeVector, MockCustomDataType);
	};
} // namespace nabi::Tests::ECS

#endif // #ifdef RUN_TESTS
