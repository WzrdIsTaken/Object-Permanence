#include "Core.h"

#include "CoreComponents\ResourceComponents\ModelResourceComponent.h"

#include "ReflectionIncludes.h"

namespace ecs::RComp
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(ModelResourceComponent)
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_MeshPath, "MeshPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_TexturePath, "TexturePath")
	REFLECT_COMPONENT_END(ModelResourceComponent)
} // namespace ecs::RComp
