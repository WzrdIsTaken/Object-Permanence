#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

namespace nabi::Physics
{
	// The result of a collision, can be calculated with CollisionSolvers::SolveCollision, and then used to resolve the collision
	struct Collision final
	{
		dx::XMFLOAT3 m_Normal;
		float m_Depth;
	};
} // namespace nabi::Physics
