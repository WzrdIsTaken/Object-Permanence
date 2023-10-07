#pragma once
#include "Core.h"

#include "Buffers\ConstantBuffer.h"
#include "ComponentBase.h"

namespace ecs::SComp
{
	struct GraphicsComponent final : public nabi::ECS::ComponentBase
	{
		std::array<nabi::Rendering::ConstantBuffer, nabi::Rendering::ConstantBufferIndex::ENUM_COUNT> m_ConstantBuffers;

		GraphicsComponent()
			: m_ConstantBuffers{}
		{
		}
	};
} // namespace ecs::SComp
