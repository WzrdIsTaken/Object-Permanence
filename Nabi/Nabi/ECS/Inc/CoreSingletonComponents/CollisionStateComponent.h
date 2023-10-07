#pragma once
#include "Core.h"

#include "ComponentBase.h"
#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"

namespace ecs::SComp
{
	struct CollisionStateComponent final : public nabi::ECS::ComponentBase
	{
		struct CollisionPair final
		{
			entt::entity m_LhsEntity;
			entt::entity m_RhsEntity;

			bool operator == (CollisionPair const& other) const
			{
				bool const sameSide     = this->m_LhsEntity == other.m_LhsEntity &&
					                      this->m_RhsEntity == other.m_RhsEntity;

				bool const oppositeSide = this->m_RhsEntity == other.m_LhsEntity &&
					                      this->m_LhsEntity == other.m_RhsEntity;

				return sameSide || oppositeSide;
			}
		};

		enum class MaxVariance : int
		{
			X = 0, 
			Y = 1, 
			Z = 2,

			ENUM_COUNT
		};

		MaxVariance m_MaxVarianceAxis;

		typedef std::vector<CollisionPair> CurrentCollisions;
		CurrentCollisions m_CurrentCollisions;

		CollisionStateComponent()
			: m_MaxVarianceAxis(MaxVariance::X)
			, m_CurrentCollisions{}
		{
		}
	};
} // namespace ecs::SComp

/*
* I was using a set for current collisions, but on reflection because of the likely small data set nabi will operate on, a vector might be the better fit
	class CollisionPairComparator final
	{
	public:
		bool operator () (CollisionPair const& lhs, CollisionPair const& rhs) const
		{
			return lhs != rhs;
		}
	};

	typedef std::set<CollisionPair, CollisionPairComparator> CurrentCollisions;
*/
