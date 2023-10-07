#pragma once
#include "Core.h"

#include "ComponentBase.h"
#include "Buffers\RenderBuffers.h"
#include "ResourceWrappers.h"

namespace ecs
{
	struct BufferComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::RenderBuffers> m_BufferResource;

		BufferComponent()
			: m_BufferResource{}
		{
		}
	};
} // namespace ecs
