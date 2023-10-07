#include "EngineCore.h"

#include "WinIncludes.h" // Undefs for min / max

#include "CollisionSolvers.h"

#include "Ray.h"

namespace nabi::Physics::CollisionSolvers
{
	AABB CreateAABBFromTopLeft(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		AABB aabb;
		ReassignAABBFromTopLeft(aabb, topLeft, dimensions);

		return aabb;
	}

	AABB CreateAABBFromCenter(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		AABB aabb;
		ReassignAABBFromCenter(aabb, topLeft, dimensions);

		return aabb;
	}

	void ReassignAABBFromTopLeft(AABB& aabb, dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;

		dx::XMFLOAT3 const minExtents = Float3Subtract(topLeft, dimensions);

		aabb.m_MinExtents = minExtents;
		aabb.m_MaxExtents = topLeft;
	}

	void ReassignAABBFromCenter(AABB& aabb, dx::XMFLOAT3 const& center, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;

		dx::XMFLOAT3 const halfDimensions = Float3Divide(dimensions, 2.0f);
		dx::XMFLOAT3 const maxExtents = Float3Add(center, halfDimensions);
		dx::XMFLOAT3 const minExtents = Float3Subtract(center, halfDimensions);

		aabb.m_MinExtents = minExtents;
		aabb.m_MaxExtents = maxExtents;
	}

	void MakeAABBIntoSphere(AABB& aabb, float const radius) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;

		dx::XMFLOAT3 const radiusVector = { radius, radius, radius };
		aabb.m_MinExtents = Float3Subtract(aabb.m_MinExtents, radiusVector);
		aabb.m_MaxExtents = Float3Add(aabb.m_MaxExtents, radiusVector);
	}

	Collision CollisionSolvers::SolveCollision(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		Collision collision = {};
		collision.m_Normal = CollisionSolvers::CalculateCollisionNormal(lhs, rhs);

		dx::XMFLOAT3 const depth = CollisionSolvers::CalculatePenetrationDepth(lhs, rhs);
		collision.m_Depth = CollisionSolvers::CalculateSmallestPentrationDepth(depth);

		return collision;
	}

	dx::XMFLOAT3 CalculateCollisionNormal(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		/*
		* This almost worked.. but caused some sliding behaviour which I couldn't quite get to the bottom of ):
		* Its probs super obvious and I'll see it as soon as I click 'commit' :p

			using namespace nabi::Utils::DirectXUtils;

			auto calculateNormalHelper =
				[](float const lhsAxisMin, float const lhsAxisMax, float const rhsAxisMin, float const rhsAxisMax) -> float
				{
					return ((rhsAxisMin + rhsAxisMax) * 0.5f) - ((lhsAxisMin + lhsAxisMax) * 0.5f);
				};

			dx::XMFLOAT3 normal = c_Float3Zero;

			normal.x = calculateNormalHelper(lhs.m_MinExtents.x, lhs.m_MaxExtents.x, rhs.m_MinExtents.x, rhs.m_MaxExtents.x);
			normal.y = calculateNormalHelper(lhs.m_MinExtents.y, lhs.m_MaxExtents.y, rhs.m_MinExtents.y, rhs.m_MaxExtents.y);
			normal.z = calculateNormalHelper(lhs.m_MinExtents.z, lhs.m_MaxExtents.z, rhs.m_MinExtents.z, rhs.m_MaxExtents.z);

			dx::XMFLOAT3 const normalizedNormal = Float3Normalize(normal);
			return normalizedNormal;
		*/

		// This collision method is kinda rough
		// What happens if the program ran slow or for whatever reason side - otherside < my arbitrary random number?
		// yeah.. not great
		// but at least it works! :)

		float constexpr collisionNormalEpsilon = 0.1f;

		float x = 0;
		if (lhs.m_MaxExtents.x - rhs.m_MinExtents.x > collisionNormalEpsilon) --x;
		if (rhs.m_MaxExtents.x - lhs.m_MinExtents.x > collisionNormalEpsilon) ++x;

		float y = 0;
		if (lhs.m_MaxExtents.y - rhs.m_MinExtents.y > collisionNormalEpsilon) --y;
		if (rhs.m_MaxExtents.y - lhs.m_MinExtents.y > collisionNormalEpsilon) ++y;

		float z = 0;
		if (lhs.m_MinExtents.z - rhs.m_MinExtents.z > collisionNormalEpsilon) --z;
		if (rhs.m_MaxExtents.z - lhs.m_MaxExtents.z > collisionNormalEpsilon) ++z;

		return { x, y, z };
	}

	bool CollisionSolvers::Intersects(AABB const& aabb, Ray const& ray, float& distance) NABI_NOEXCEPT
	{
		// Resources:
		// https://tavianator.com/2015/ray_box_nan.html
		// https://gamedev.stackexchange.com/a/18459 

		dx::XMFLOAT3 inverseDirection = nabi::DirectXUtils::c_Float3Zero;
		inverseDirection.x = 1.0f / ray.m_Direction.x;
		inverseDirection.y = 1.0f / ray.m_Direction.y;
		inverseDirection.z = 1.0f / ray.m_Direction.z;

		float const t1 = (aabb.m_MinExtents.x - ray.m_Origin.x) * inverseDirection.x;
		float const t2 = (aabb.m_MaxExtents.x - ray.m_Origin.x) * inverseDirection.x;
		float const t3 = (aabb.m_MinExtents.y - ray.m_Origin.y) * inverseDirection.y;
		float const t4 = (aabb.m_MaxExtents.y - ray.m_Origin.y) * inverseDirection.y;
		float const t5 = (aabb.m_MinExtents.z - ray.m_Origin.z) * inverseDirection.z;
		float const t6 = (aabb.m_MaxExtents.z - ray.m_Origin.z) * inverseDirection.z;

		float const tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float const tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tMax < 0.0f)
		{
			distance = tMax;
			return false;
		}

		if (tMin > tMax)
		{
			distance = tMax;
			return false;
		}

		distance = tMin;
		return true;
	}

	dx::XMFLOAT3 CalculatePenetrationDepth(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		using namespace nabi::DirectXUtils;

		/*
			auto calculatePentrationHelper =
				[](float const lhsAxisMin, float const lhsAxisMax, float const rhsAxisMin, float const rhsAxisMax) -> float
				{
					return std::min(lhsAxisMax, rhsAxisMax) - std::max(lhsAxisMin, rhsAxisMin);
				};

			dx::XMFLOAT3 depth = c_Float3Zero;

			depth.x = calculatePentrationHelper(lhs.m_MinExtents.x, lhs.m_MaxExtents.x, rhs.m_MinExtents.x, rhs.m_MaxExtents.x);
			depth.y = calculatePentrationHelper(lhs.m_MinExtents.y, lhs.m_MaxExtents.y, rhs.m_MinExtents.y, rhs.m_MaxExtents.y);
			depth.z = calculatePentrationHelper(lhs.m_MinExtents.z, lhs.m_MaxExtents.z, rhs.m_MinExtents.z, rhs.m_MaxExtents.z);

			return depth;
		*/

		dx::XMFLOAT3 depth = c_Float3Zero;

		depth.x = std::min(lhs.m_MaxExtents.x, rhs.m_MaxExtents.x) - std::max(lhs.m_MinExtents.x, rhs.m_MinExtents.x);
		depth.y = std::min(lhs.m_MaxExtents.y, rhs.m_MaxExtents.y) - std::max(lhs.m_MinExtents.y, rhs.m_MinExtents.y);
		depth.z = std::min(lhs.m_MaxExtents.z, rhs.m_MaxExtents.z) - std::max(lhs.m_MinExtents.z, rhs.m_MinExtents.z);

		return depth;
	}

	float CalculateSmallestPentrationDepth(dx::XMFLOAT3 const& penetrationDepth) NABI_NOEXCEPT
	{
		float penetration = 0.0f;

		if (penetrationDepth.x < penetrationDepth.y && penetrationDepth.x < penetrationDepth.z) 
		{
			penetration = penetrationDepth.x;
		}
		else if (penetrationDepth.y < penetrationDepth.z)
		{
			penetration = penetrationDepth.y;
		}
		else 
		{
			penetration = penetrationDepth.z;
		}

		return penetration;
	}

	std::string AABBToString(AABB const& aabb, std::optional<std::string> const aabbName) NABI_NOEXCEPT
	{
		std::string const maxExtents = 
			std::to_string(aabb.m_MaxExtents.x) + " " + 
			std::to_string(aabb.m_MaxExtents.y) + " " + 
			std::to_string(aabb.m_MaxExtents.z);

		std::string const minExtents =
			std::to_string(aabb.m_MinExtents.x) + " " +
			std::to_string(aabb.m_MinExtents.y) + " " +
			std::to_string(aabb.m_MinExtents.z);

		std::string const name = aabbName.has_value() ? aabbName.value() : "";

		return name + " Max: " + maxExtents + " | Min: " + minExtents;
	}
} // namespace nabi::Physics::CollisionSolvers
