#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

namespace nabi::Physics
{
	// Used for Raycasting
	struct Ray final
	{
		dx::XMFLOAT3 m_Origin;
		dx::XMFLOAT3 m_Direction;
	};
} // namespace nabi::Physics
