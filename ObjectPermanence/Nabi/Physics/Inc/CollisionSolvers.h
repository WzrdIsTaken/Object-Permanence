#pragma once
#include "EngineCore.h"

#include <optional>

#include "DirectXIncludes.h"

#include "AABB.h"
#include "Collision.h"
#include "DirectXUtils.h"

namespace nabi::Physics
{
	struct Ray;
} // namespace nabi::Physics

namespace nabi::Physics::CollisionSolvers
{
	[[nodiscard]] AABB CreateAABBFromTopLeft(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	[[nodiscard]] AABB CreateAABBFromCenter(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	void ReassignAABBFromTopLeft(AABB& aabb, dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	void ReassignAABBFromCenter(AABB& aabb, dx::XMFLOAT3 const& center, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;

	void MakeAABBIntoSphere(AABB& aabb, float const radius) NABI_NOEXCEPT;

	[[nodiscard]] inline dx::XMFLOAT3 GetCenter(AABB const& aabb) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;
		return Float3Multiply(Float3Add(aabb.m_MinExtents, aabb.m_MaxExtents), 0.5f);
	}
	[[nodiscard]] inline dx::XMFLOAT3 GetExtents(AABB const& aabb) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;
		return Float3Multiply(Float3Subtract(aabb.m_MaxExtents, aabb.m_MinExtents), 0.5f);
	}

	[[nodiscard]] inline bool Intersects(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		/*
			if (lhs.m_MaxExtents.x <= rhs.m_MinExtents.x || lhs.m_MinExtents.x >= rhs.m_MaxExtents.x) return false;
			if (lhs.m_MaxExtents.y <= rhs.m_MinExtents.y || lhs.m_MinExtents.y >= rhs.m_MaxExtents.y) return false;
			if (lhs.m_MaxExtents.z <= rhs.m_MinExtents.z || lhs.m_MinExtents.z >= rhs.m_MaxExtents.z) return false;

			return true;
		*/

		using namespace nabi::DirectXUtils;
		return Float3LessOrEqual(lhs.m_MinExtents, rhs.m_MaxExtents) && Float3GreaterOrEqual(lhs.m_MaxExtents, rhs.m_MinExtents);
	}
	[[nodiscard]] bool Intersects(AABB const& aabb, Ray const& ray, float& distance) NABI_NOEXCEPT;

	[[nodiscard]] Collision SolveCollision(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT; // <-- the mvp
	[[nodiscard]] dx::XMFLOAT3 CalculateCollisionNormal(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT;
	[[nodiscard]] dx::XMFLOAT3 CalculatePenetrationDepth(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT;
	[[nodiscard]] float CalculateSmallestPentrationDepth(dx::XMFLOAT3 const& penetrationDepth) NABI_NOEXCEPT;

	[[nodiscard]] std::string AABBToString(AABB const& aabb, std::optional<std::string> const aabbName = std::nullopt) NABI_NOEXCEPT;
} // namespace nabi::Physics::CollisionSolvers
