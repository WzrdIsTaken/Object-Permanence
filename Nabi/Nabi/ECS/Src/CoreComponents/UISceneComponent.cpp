#include "Core.h"

#include "CoreComponents\UISceneComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(UISceneComponent)
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_SceneName, "SceneName")
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_UISceneEntityNames, "SceneEntities")
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_Enabled, "Enabled")
	REFLECT_COMPONENT_END(UISceneComponent)
} // namespace ecs

