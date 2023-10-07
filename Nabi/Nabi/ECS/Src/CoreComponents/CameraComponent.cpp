#include "Core.h"

#include "CoreComponents\CameraComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(CameraComponent)
		REFLECT_COMPONENT_PROPERTY(CameraComponent::m_Position, "Position")
		REFLECT_COMPONENT_PROPERTY(CameraComponent::m_Target, "Target")
		REFLECT_COMPONENT_PROPERTY(CameraComponent::m_HasToBeUpdated, "StartAwake")
	REFLECT_COMPONENT_END(CameraComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(CameraGroupComponent)
	REFLECT_COMPONENT_END(CameraGroupComponent)
} // namespace ecs
