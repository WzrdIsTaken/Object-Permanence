#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	struct AsteroidComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_MoveDirection;
		float m_MoveSpeed;
		dx::XMFLOAT3 m_SpinDirection;
		float m_SpinSpeed;

		AsteroidComponent()
			: m_MoveDirection{ 0.0f, 0.0f, 0.0f }
			, m_MoveSpeed(0.0f)
			, m_SpinDirection{ 0.0f, 0.0f, 0.0f }
			, m_SpinSpeed(0.0f)
		{
		}
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
