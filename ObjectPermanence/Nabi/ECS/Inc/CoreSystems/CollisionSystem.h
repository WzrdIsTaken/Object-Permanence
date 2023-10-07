#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "CoreSingletonComponents\CollisionStateComponent.h"
#include "SystemBase.h"

namespace ecs
{
	struct ColliderComponent;
	struct RigidbodyComponent;
	struct TransformComponent;
} // namespace ecs
namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi
namespace nabi::Physics
{
	struct AABB;
	struct Collision;
} // namespace nabi::Physics

namespace ecs
{
	class CollisionSystem final : public nabi::ECS::SystemBase
	{
	public:
		CollisionSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~CollisionSystem();

		void FixedUpdate(nabi::GameTime const& gameTime);

	private:
		struct CollisionEventData final
		{
			entt::entity const m_Entity;
			nabi::Physics::AABB const& m_AABB;

			ColliderComponent const& m_ColliderComponent;
			RigidbodyComponent& m_RigidbodyComponent;
			TransformComponent& m_TransformComponent;
		};

		enum class CollisionState : int
		{
			Colliding,
			NotColliding,

			ENUM_COUNT
		};

		void BroadPhase(float const dt) const;
		void NarrowPhase(float const dt, CollisionEventData& lhsData, CollisionEventData& rhsData) const;

		void FireCollisionEvents(CollisionState const collisionState, CollisionEventData const& lhsData, CollisionEventData const& rhsData) const;
		void ResolveCollision(float const dt, nabi::Physics::Collision const& collision, CollisionEventData& data) const;

		void CalculateNextMaxVariance(size_t const numberOfColliders, dx::XMFLOAT3 const& centerSum, dx::XMFLOAT3 const& centerSumSquared) const;

		REFLECT_PRIVATES(CollisionSystem)
	};
} // namespace ecs
