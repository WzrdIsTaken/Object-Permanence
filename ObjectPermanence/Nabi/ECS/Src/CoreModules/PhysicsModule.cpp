#include "Core.h"

#include "CoreModules\PhysicsModule.h"

#include "AABB.h"
#include "CollisionSolvers.h"
#include "Ray.h"

#define USE_RAYCAST_HEURISTIC 

namespace ecs::PhysicsModule
{
	namespace
	{
		[[nodiscard]] inline bool IsCollisionViewValidForSorting(CollisionView const& view) noexcept
		{
			// We could use size_hint()... but I don't know how accurate the estimate is (+ this can crash if its not accurate!)
			return view && std::distance(view.begin(), view.end()) > 1u;
		}
	}

	nabi::Physics::RaycastHitResult Raycast(nabi::Context& context, dx::XMFLOAT3 const& origin, dx::XMFLOAT3 const& direction, RaycastSettings const& settings)
	{
		int constexpr numberOfResults = 1;
		nabi::Physics::RaycastHitResult const result = Raycast(context, origin, direction, numberOfResults, settings).front();

		return result;
	}

	std::vector<nabi::Physics::RaycastHitResult> Raycast(nabi::Context& context, dx::XMFLOAT3 const& origin, dx::XMFLOAT3 const& direction, int const numberOfResults, RaycastSettings const& settings)
	{
		using namespace nabi::Physics;

		std::vector<RaycastHitResult> results = {};
#ifdef USE_RAYCAST_HEURISTIC
		results.reserve(numberOfResults);
#endif // ifdef USE_RAYCAST_HEURISTIC 

		Ray ray;
		ray.m_Origin = origin;
		ray.m_Direction = direction;

		float distance = FLT_MAX;

		AABB lhsAABB = {};
		AABB rhsAABB = {};

		SComp::CollisionStateComponent::MaxVariance const comparisonAxis = GetCollisionStateComponent(context).m_MaxVarianceAxis;
		auto view = GetSortedCollisionView(context, comparisonAxis, lhsAABB, rhsAABB);

		for (auto [entity, transformComponent, rigidbodyComponent, colliderComponent] : view.each())
		{
			bool const validMask = ValidCollisionMask(context, colliderComponent.m_Mask, settings.m_Mask);
			if (validMask)
			{
				ReassignAABBFromCollisionComponents(lhsAABB, transformComponent, colliderComponent);
				bool const intersects = CollisionSolvers::Intersects(lhsAABB, ray, distance);

				if (distance > settings.m_Range)
				{
					// No more AABBs are in range
					break;
				}

				if (intersects)
				{
					RaycastHitResult result = c_EmptyRaycastResult;
					result.m_Entity = entity;
					result.m_Distance = distance;
					results.emplace_back(result);

					if (results.size() == numberOfResults)
					{
						break;
					}
				}
			}
		}

		if (results.empty())
		{
			RaycastHitResult result = c_EmptyRaycastResult;
			results.emplace_back(result);
		}

		return results;
	}

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis)
	{
		nabi::Physics::AABB lhsAABB = {};
		nabi::Physics::AABB rhsAABB = {};

		return GetSortedCollisionView(context, comparisonAxis, lhsAABB, rhsAABB);
	}

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis,
		nabi::Physics::AABB& lhsAABB, nabi::Physics::AABB& rhsAABB)
	{
		auto view = context.m_Registry.view<TransformComponent, RigidbodyComponent, ColliderComponent const>();
		if (IsCollisionViewValidForSorting(view))
		{
			// i think i need to improve my knowledge of storages.
			// i originally thought that view.storage was the components of type T inside the view, but that doesn't seem to be the case..
			// this is a quick hack to get this working. i just try_get the collider and if its null, i guess the entity is just one
			// which has a transform component but is not in the collision view. this isn't great.. but is fine for now.
			// the simulation is on its own thread anyway :p 
			// (see the original code at the bottom of this file)

			view.storage<TransformComponent const>().sort(
				[&context, &view, comparisonAxis, &lhsAABB, &rhsAABB](entt::entity const lhs, entt::entity const rhs) -> bool
				{
					bool lhsCloser = false;

					ColliderComponent const* const lhsCollider = context.m_Registry.try_get<ColliderComponent>(lhs);
					ColliderComponent const* const rhsCollider = context.m_Registry.try_get<ColliderComponent>(rhs);

					if (lhsCollider && rhsCollider)
					{
						TransformComponent const& lhsTransform = view.get<TransformComponent>(lhs);
						TransformComponent const& rhsTransform = view.get<TransformComponent>(rhs);

						ReassignAABBFromCollisionComponents(lhsAABB, lhsTransform, *lhsCollider);
						ReassignAABBFromCollisionComponents(rhsAABB, rhsTransform, *rhsCollider);

						float const lhsMaxVariance = GetVarianceValue(lhsAABB.m_MinExtents, comparisonAxis);
						float const rhsMaxVariance = GetVarianceValue(rhsAABB.m_MinExtents, comparisonAxis);

						lhsCloser = lhsMaxVariance < rhsMaxVariance;
					}

					return lhsCloser;
				});
			view = view.use<TransformComponent>();
		}
		
		return view;
	}

	void ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb,
		TransformComponent const& transformComponent, ColliderComponent const& colliderComponent)
	{
		dx::XMFLOAT3 const& center = transformComponent.m_Position;
		dx::XMFLOAT3 const& dimensions = colliderComponent.m_ColliderDimensions;

		using namespace nabi::Physics;
		CollisionSolvers::ReassignAABBFromCenter(aabb, center, dimensions);

		switch (colliderComponent.m_ColliderType)
		{
		case ColliderComponent::ColliderType::Cube:
			// No special case 
			break;
		case ColliderComponent::ColliderType::Sphere:
		{
			ASSERT(dimensions.x == dimensions.y && dimensions.y == dimensions.z,
				"For a sphere collider, expecting x/y/z dimensions to be equal");

			float const radius = dimensions.x / 2.0f;
			CollisionSolvers::MakeAABBIntoSphere(aabb, radius);
			break;
		}
		default:
			ASSERT_FAIL("Using an unexpected ColliderComponent::ColliderType");
			break;
		}
	}

	float GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance)
	{
		// high quality function

		using namespace nabi::TypeUtils;
		int const maxVarianceIndex = ToUnderlying<SComp::CollisionStateComponent::MaxVariance>(variance);

		ASSERT_FATAL(maxVarianceIndex == 0 || maxVarianceIndex == 1 || maxVarianceIndex == 2,
			"Indexing into a float3 with a value of " << maxVarianceIndex << " is *not* going to go well.");

		char* const ptr = reinterpret_cast<char*>(&const_cast<dx::XMFLOAT3&>(extent));
		float const maxVariance = *reinterpret_cast<float*>(ptr + sizeof(float) * maxVarianceIndex);

		return maxVariance;
	}
} // namespace ecs::PhysicsModule

/*
* Original GetSortedCollisionView() code:
* (I think that the IsCollisionViewValidForSorting() is an ok addition, but I'm still not 100% sure about the storage stuff)

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis,
		nabi::Physics::AABB& lhsAABB, nabi::Physics::AABB& rhsAABB)
	{
		auto view = context.m_Registry.view<TransformComponent, RigidbodyComponent, ColliderComponent const>();
		view.storage<TransformComponent const>().sort(
			[&context, comparisonAxis, &lhsAABB, &rhsAABB](entt::entity const lhs, entt::entity const rhs) -> bool
			{
				TransformComponent const& lhsTransform = context.m_Registry.get<TransformComponent>(lhs);
				TransformComponent const& rhsTransform = context.m_Registry.get<TransformComponent>(rhs);

				ColliderComponent const& lhsCollider = context.m_Registry.get<ColliderComponent>(lhs);
				ColliderComponent const& rhsCollider = context.m_Registry.get<ColliderComponent>(rhs);

				ReassignAABBFromCollisionComponents(lhsAABB, lhsTransform, lhsCollider);
				ReassignAABBFromCollisionComponents(rhsAABB, rhsTransform, rhsCollider);

				float const lhsMaxVariance = GetVarianceValue(lhsAABB.m_MinExtents, comparisonAxis);
				float const rhsMaxVariance = GetVarianceValue(rhsAABB.m_MinExtents, comparisonAxis);

				return lhsMaxVariance < rhsMaxVariance;
			});
		view = view.use<TransformComponent>();

		return view;
	}
*/
