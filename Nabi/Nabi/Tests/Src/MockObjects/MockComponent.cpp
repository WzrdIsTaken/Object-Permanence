#include "Core.h"

#include "MockObjects/MockComponent.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ECS
{
	REFLECT_COMPONENT_BEGIN(MockComponent, "MockComponent")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_IntType, "IntType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_FloatType, "FloatType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_StringType, "StringType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_CustomType, "CustomType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_Enum, "Enum")
	REFLECT_COMPONENT_END(MockComponent)

	REFLECT_COMPONENT_BEGIN(MockComponentWithDirectXTypes, "MockComponentWithDirectXTypes")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithDirectXTypes::m_Float2, "Float2")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithDirectXTypes::m_Float3, "Float3")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithDirectXTypes::m_Int2, "Int2")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithDirectXTypes::m_Int3, "Int3")
	REFLECT_COMPONENT_END(MockComponentWithDirectXTypes)

	REFLECT_CONTAINER(m_IntVector, MockComponentWithContainers)
	REFLECT_CONTAINER(m_StringVector, MockComponentWithContainers)
	REFLECT_CONTAINER(m_IntMap, MockComponentWithContainers)
	//REFLECT_CONTAINER(m_CustomTypeVector, MockComponentWithContainers)

	REFLECT_COMPONENT_BEGIN(MockComponentWithContainers, "MockComponentWithContainers")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithContainers::m_IntVector, "IntVector")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithContainers::m_StringVector, "StringVector")
		REFLECT_COMPONENT_PROPERTY(MockComponentWithContainers::m_IntMap, "IntMap")
		//REFLECT_COMPONENT_PROPERTY(MockComponentWithContainers::m_CustomTypeVector, "CustomTypeVector")
	REFLECT_COMPONENT_END(MockComponentWithContainers)
} // namespace nabi::Tests::ECS

#endif // #ifdef RUN_TESTS
