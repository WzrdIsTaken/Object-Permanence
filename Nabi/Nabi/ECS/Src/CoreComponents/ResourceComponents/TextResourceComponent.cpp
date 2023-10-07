#include "Core.h"

#include "CoreComponents\ResourceComponents\TextResourceComponent.h"

#include "ReflectionIncludes.h"

namespace ecs::RComp
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(TextResourceComponent)
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_FontPath, "FontPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_Content, "Content")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_CharacterSpace, "CharacterSpace")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_AsciiShift, "AsciiShift")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_TextureAtlas, "TextureAtlas")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_CharacterPoolSize, "CharacterPoolSize")
	REFLECT_COMPONENT_END(TextResourceComponent)
} // namespace ecs::RComp
