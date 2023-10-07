#include "Core.h"

#include "CoreComponents\ColliderComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	typedef ColliderComponent::ColliderType ColliderType;
	REFLECT_ENUM_BEGIN_DEFAULT(ColliderType)
		REFLECT_ENUM_VALUE_DEFAULT(ColliderType::Cube)
		REFLECT_ENUM_VALUE_DEFAULT(ColliderType::Sphere)
	REFLECT_ENUM_END(ColliderType)

	typedef ColliderComponent::InteractionType InteractionType;
	REFLECT_ENUM_BEGIN_DEFAULT(InteractionType)
		REFLECT_ENUM_VALUE_DEFAULT(InteractionType::Dynamic)
		REFLECT_ENUM_VALUE_DEFAULT(InteractionType::Static)
	REFLECT_ENUM_END(InteractionType)

	REFLECT_COMPONENT_BEGIN_DEFAULT(ColliderComponent)
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_ColliderType, "ColliderType")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_ColliderDimensions, "ColliderDimensions")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_Mask, "Mask")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_InteractionType, "InteractionType")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_OnCollisionEnterType, "OnCollisionEnterType")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_OnCollisionEnterAction, "OnCollisionEnterAction")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_OnCollisionExitType, "OnCollisionExitType")
		REFLECT_COMPONENT_PROPERTY(ColliderComponent::m_OnCollisionExitAction, "OnCollisionExitAction")
	REFLECT_COMPONENT_END(ColliderComponent)
} // namespace ecs
