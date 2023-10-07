#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Components\AsteroidComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(AsteroidComponent)
		REFLECT_COMPONENT_PROPERTY(AsteroidComponent::m_MoveDirection, "MoveDirection")
		REFLECT_COMPONENT_PROPERTY(AsteroidComponent::m_MoveSpeed, "MoveSpeed")
		REFLECT_COMPONENT_PROPERTY(AsteroidComponent::m_SpinDirection, "SpinDirection")
		REFLECT_COMPONENT_PROPERTY(AsteroidComponent::m_SpinSpeed, "SpinSpeed")
	REFLECT_COMPONENT_END(AsteroidComponent)
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
