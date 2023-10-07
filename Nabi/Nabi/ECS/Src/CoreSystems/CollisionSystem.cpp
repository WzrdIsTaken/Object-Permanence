#include "Core.h"

#include "CoreSystems\CollisionSystem.h"

#include "Collision.h"
#include "CollisionSolvers.h"
#include "Context.h"
#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\PhysicsModule.h"
#include "CoreModules\ReflectionModule.h"
#include "DirectXUtils.h"
#include "ECSUtils.h"
#include "GameTime.h"

// For some games, narrow phase collision resolution might not be needed - just collision detection
#define ENABLE_NARROW_PHASE 

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(CollisionSystem)
	RELFECT_SYSTEM_END(CollisionSystem)

	CollisionSystem::CollisionSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(CollisionSystem)
	}

	CollisionSystem::~CollisionSystem()
	{
		UNREGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(CollisionSystem)
	}

	void CollisionSystem::FixedUpdate(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetFixedDeltaTime());
		BroadPhase(dt);
	}

	// 

	void CollisionSystem::BroadPhase(float const dt) const
	{
		// Sweep and prune collision detection

		using namespace nabi::Physics;

		// Sweep along the comparison axis. This axis (x, y or z) is the axis on which in the previous frame there was the largest
		// distance between colliders. Using this axis to sweep can help ensure that the spatial partitioning is balanced 
		// and reduce unnecessary intersection tests.
		SComp::CollisionStateComponent::MaxVariance const comparisonAxis = PhysicsModule::GetCollisionStateComponent(m_Context).m_MaxVarianceAxis;

		// Left and right hand side AABBs, used throughout the loop
		AABB lhsAABB = {};
		AABB rhsAABB = {};

		// Sort the view by the AABB's transforms along the comparison axis
		auto view = PhysicsModule::GetSortedCollisionView(m_Context, comparisonAxis, lhsAABB, rhsAABB).each();

		// The center is used to calculate the next comparison axis
		dx::XMFLOAT3 centerSum = nabi::DirectXUtils::c_Float3Zero;
		dx::XMFLOAT3 centerSumSquared = nabi::DirectXUtils::c_Float3Zero;

		// Iterate through each collider
		size_t iterationProgress = 1u;
		for (auto lhsIt = view.begin(); lhsIt != view.end(); ++lhsIt)
		{
			// Get the AABB lhs collider
			auto [lhsEntity, lhsTransformComponent, lhsRigidbodyComponent, lhsColliderComponent] = *lhsIt;
			PhysicsModule::ReassignAABBFromCollisionComponents(lhsAABB, lhsTransformComponent, lhsColliderComponent);

			dx::XMFLOAT3 const center = CollisionSolvers::GetCenter(lhsAABB);
			centerSum = nabi::DirectXUtils::Float3Add(centerSum, center);
			centerSumSquared = nabi::DirectXUtils::Float3Add(centerSumSquared, nabi::DirectXUtils::Float3Square(center));

			// Iterate through each collider in front of the lhs one
			auto rhsIt = view.begin();
			rhsIt = std::next(rhsIt, iterationProgress);

			for (; rhsIt != view.end(); ++rhsIt)
			{
				// Check if the two colliders have a valid mask to check collisions
				auto [rhsEntity, rhsTransformComponent, rhsRigidbodyComponent, rhsColliderComponent] = *rhsIt;
				bool const validMask = PhysicsModule::ValidCollisionMask(m_Context, lhsColliderComponent.m_Mask, rhsColliderComponent.m_Mask);

				if (validMask)
				{
					// Get the AABB rhs collider
					PhysicsModule::ReassignAABBFromCollisionComponents(rhsAABB, rhsTransformComponent, rhsColliderComponent);
					LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, 
						CollisionSolvers::AABBToString(lhsAABB, "LHS:") + " | " + CollisionSolvers::AABBToString(rhsAABB, "RHS:"), LOG_END);

					// Check if the AABBs are intersecting
					dx::XMFLOAT3 const& otherMinExtents = lhsAABB.m_MinExtents;
					dx::XMFLOAT3 const& thisMaxExtents = rhsAABB.m_MaxExtents;
					float const otherMinExtent = PhysicsModule::GetVarianceValue(otherMinExtents, comparisonAxis);
					float const thisMaxExtent = PhysicsModule::GetVarianceValue(thisMaxExtents, comparisonAxis);

					bool const otherExtentBeyondThisExtent = otherMinExtent > thisMaxExtent;
					if (otherExtentBeyondThisExtent)
					{
						// No more collisions are possible with this AABB
						break;
					}

					bool const aabbsIntersect = CollisionSolvers::Intersects(lhsAABB, rhsAABB);
					CollisionState collisionState = CollisionState::NotColliding;

					CollisionEventData lhsCollisionEventData = {
						lhsEntity, lhsAABB, lhsColliderComponent, lhsRigidbodyComponent, lhsTransformComponent
					};
					CollisionEventData rhsCollisionEventData = {
						rhsEntity, rhsAABB, rhsColliderComponent, rhsRigidbodyComponent, rhsTransformComponent
					};

					if (aabbsIntersect)
					{
						ASSERT_CODE
						(
							static uint64_t collisionCount = 0ull;
							++collisionCount;
							LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, "Broad Phase - Collision! Count: " << collisionCount, LOG_END);
						)

#ifdef ENABLE_NARROW_PHASE
						NarrowPhase(dt, lhsCollisionEventData, rhsCollisionEventData);
#endif // ifdef ENABLE_NARROW_PHASE

						// Set the collision state to colliding, used below for firing collision events
						collisionState = CollisionState::Colliding;
					}

					FireCollisionEvents(collisionState, lhsCollisionEventData, rhsCollisionEventData);
				}
			}

			++iterationProgress;
		}

		CalculateNextMaxVariance(iterationProgress, centerSum, centerSumSquared);
	}

	void CollisionSystem::NarrowPhase(float const dt, CollisionEventData& lhsData, CollisionEventData& rhsData) const
	{
		using namespace nabi::Physics;

		if (lhsData.m_ColliderComponent.m_InteractionType == ColliderComponent::InteractionType::Dynamic)
		{
			Collision const collision = CollisionSolvers::SolveCollision(rhsData.m_AABB, lhsData.m_AABB);
			ResolveCollision(dt, collision, lhsData);
		}
		if (rhsData.m_ColliderComponent.m_InteractionType == ColliderComponent::InteractionType::Dynamic)
		{
			Collision const collision = CollisionSolvers::SolveCollision(lhsData.m_AABB, rhsData.m_AABB);
			ResolveCollision(dt, collision, rhsData);
		}
	}

	void CollisionSystem::FireCollisionEvents(CollisionState const collisionState, CollisionEventData const& lhsData, CollisionEventData const& rhsData) const
	{
		ASSERT_CODE(using namespace nabi::ECSUtils;)

		auto fireCollisionEventsHelper =
			[&](entt::hashed_string const& actionType, entt::hashed_string const& actionName, entt::entity const lhsEntity, entt::entity const rhsEntity) -> void
			{
				ReflectionModule::Constraints constexpr constraints = ReflectionModule::c_EventConstraints;
				ReflectionModule::CallReflectedFunction(m_Context, actionType, actionName, &constraints, entt::forward_as_meta(m_Context), lhsEntity, rhsEntity);
			};

		SComp::CollisionStateComponent::CurrentCollisions& currentCollisions = PhysicsModule::GetCollisionStateComponent(m_Context).m_CurrentCollisions;
		SComp::CollisionStateComponent::CollisionPair const collisionPair =
		{
			lhsData.m_Entity,
			rhsData.m_Entity,
		};

		ColliderComponent const& lhsCollider = lhsData.m_ColliderComponent;
		ColliderComponent const& rhsCollider = rhsData.m_ColliderComponent;
		
		auto it = std::find(currentCollisions.begin(), currentCollisions.end(), collisionPair);
		bool const presentInCurrentCollisions = it != currentCollisions.end();

		if (collisionState == CollisionState::Colliding         && !presentInCurrentCollisions)
		{
			LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, "Collision Enter: " <<
				GetEntityUUIDAsString(lhsData.m_Entity) << "-" << GetEntityUUIDAsString(rhsData.m_Entity), LOG_END);

			currentCollisions.emplace_back(collisionPair);

			fireCollisionEventsHelper(lhsCollider.m_OnCollisionEnterType, lhsCollider.m_OnCollisionEnterAction, lhsData.m_Entity, rhsData.m_Entity);
			fireCollisionEventsHelper(rhsCollider.m_OnCollisionEnterType, rhsCollider.m_OnCollisionEnterAction, rhsData.m_Entity, lhsData.m_Entity);
		}
		else if (collisionState == CollisionState::NotColliding && presentInCurrentCollisions)
		{
			LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, "Collision Exit: " <<
				GetEntityUUIDAsString(lhsData.m_Entity) << "-" << GetEntityUUIDAsString(rhsData.m_Entity), LOG_END);

			currentCollisions.erase(it);

			fireCollisionEventsHelper(lhsCollider.m_OnCollisionExitType, lhsCollider.m_OnCollisionExitAction, lhsData.m_Entity, rhsData.m_Entity);
			fireCollisionEventsHelper(rhsCollider.m_OnCollisionExitType, rhsCollider.m_OnCollisionExitAction, rhsData.m_Entity, lhsData.m_Entity);
		}
	}

	void CollisionSystem::ResolveCollision(float const dt, nabi::Physics::Collision const& collision, CollisionEventData& data) const
	{
		dx::XMFLOAT3 const resultant = nabi::DirectXUtils::Float3Multiply(collision.m_Normal, collision.m_Depth);
		float const inverseDt = 1.0f / dt;
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, "Narrow Phase - Normal: " << nabi::DirectXUtils::Float3ToString(collision.m_Normal) <<
			" | Depth: " << collision.m_Depth << " | Normal * Depth = " << nabi::DirectXUtils::Float3ToString(resultant), LOG_END);

		// Transform adjustment
		TransformComponent& transform = data.m_TransformComponent;
		transform.m_Position = nabi::DirectXUtils::Float3Add(transform.m_Position, resultant);

		// Rigidbody adjustment 
		RigidbodyComponent& rigidbody = data.m_RigidbodyComponent;
		dx::XMFLOAT3 const velocityChange = nabi::DirectXUtils::Float3Multiply(resultant, inverseDt);
		rigidbody.m_Velocity = nabi::DirectXUtils::Float3Add(rigidbody.m_Velocity, velocityChange);
	}

	//

	void CollisionSystem::CalculateNextMaxVariance(size_t const numberOfColliders, dx::XMFLOAT3 const& centerSum, dx::XMFLOAT3 const& centerSumSquared) const
	{
		float const numberOfCollidersAsFloat = static_cast<float>(numberOfColliders);
		dx::XMFLOAT3 const centerSumDivSize = nabi::DirectXUtils::Float3Divide(centerSum, numberOfCollidersAsFloat);
		dx::XMFLOAT3 const centerSumSquaredDivSize = nabi::DirectXUtils::Float3Divide(centerSumSquared, numberOfCollidersAsFloat);

		dx::XMFLOAT3 const variance = nabi::DirectXUtils::Float3Subtract(centerSumSquaredDivSize, nabi::DirectXUtils::Float3Square(centerSumDivSize));
		float maxVariance = variance.x;
		auto maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::X;

		if (variance.y > maxVariance)
		{
			maxVariance = variance.y;
			maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::Y;
		}
		if (variance.z > maxVariance)
		{
			maxVariance = variance.z;
			maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::Z;
		}

		PhysicsModule::GetCollisionStateComponent(m_Context).m_MaxVarianceAxis = maxVarianceAxis;
	}

} // namespace ecs

/*
* If I want to implement physics materials in the future, this could be a way of doing it. Basically have a reflected function which is called
* when a collision is detected. But, currently this is solving a problem which doesn't + might never exist - so I'm not going to worry about it. 
* 
	void CollisionSystem::FirePhysicsMaterialEvent(float const dt, nabi::Physics::Collision const& collision, CollisionEventData& data) const
	{
		ReflectionModule::Constraints constexpr constraints = ReflectionModule::c_EventConstraints;
		ColliderComponent const& colliderComponent = data.m_ColliderComponent;

		ReflectionModule::CallReflectedFunction(m_Context, 
			colliderComponent.m_PhysicsMaterialType, colliderComponent.m_PhysicsMaterialAction, 
			&constraints, entt::forward_as_meta(m_Context), collision, dt, data.m_RigidbodyComponent, data.m_Entity);
	}
*/

/*
* Could be useful in the future? 
* I used this in NarrowPhase() before the refactor
* 
	ASSERT_CODE
	(

		using namespace nabi::Utils::DirectXUtils;
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION, "Narrow Phase - LHS Collision Normal: " << Float3ToString(lhsCollision.m_Normal)
			<< " | Depth: " << Float3ToString(lhsCollisionDepth) <<    " - RHS Collision Normal: " << Float3ToString(rhsCollision.m_Normal)
			<< " | Depth: " << Float3ToString(rhsCollisionDepth), LOG_END);
	)
*/
