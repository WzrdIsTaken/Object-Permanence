#include "Core.h"

#include "CoreComponents\LightComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(DirectionalLightComponent)
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Direction, "Direction")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Colour, "Colour")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Intensity, "Intensity")

		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_AmbientIntensity, "AmbientIntensity")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_DiffuseIntensity, "DiffuseIntensity")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_SpecularAttenuation, "SpecularAttenuation")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_SpecularIntensity, "SpecularIntensity")
	REFLECT_COMPONENT_END(DirectionalLightComponent)
} // namespace ecs
