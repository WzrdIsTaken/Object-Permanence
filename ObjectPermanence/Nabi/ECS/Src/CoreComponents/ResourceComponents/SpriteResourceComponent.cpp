#include "Core.h"

#include "CoreComponents\ResourceComponents\SpriteResourceComponent.h"

#include "ReflectionIncludes.h"

namespace ecs::RComp
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(SpriteResourceComponent)
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_ImagePath, "ImagePath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_UVs, "UVs")
	REFLECT_COMPONENT_END(SpriteResourceComponent)
} // namespace ecs::RComp
