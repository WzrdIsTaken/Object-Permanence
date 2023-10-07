#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	struct TransformComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Rotation; // Stored as euler, converted to quanterion in RenderSystem / wherever else its needed
		dx::XMFLOAT3 m_Scale;

		TransformComponent()
			: m_Position{}
			, m_Rotation{}
			, m_Scale{1.0f, 1.0f, 1.0f}
		{
		}
	};
} // namespace ecs
