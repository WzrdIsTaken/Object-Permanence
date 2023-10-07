#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

namespace nabi::Physics
{
	// All collisions in Nabi are currently done via AABB
	// Perhaps this will change in the future - obj collision?
	struct AABB final
	{
		dx::XMFLOAT3 m_MinExtents;
		dx::XMFLOAT3 m_MaxExtents;
	};
} // namespace nabi::Physics
