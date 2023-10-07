#pragma once
#include "Core.h"

#include "ComponentBase.h"
#include "Textures\Texture.h"
#include "ResourceWrappers.h"

namespace ecs
{
	struct TextureComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::Texture> m_TextureResource;

		TextureComponent()
			: m_TextureResource{}
		{
		}
	};
} // namespace ecs
