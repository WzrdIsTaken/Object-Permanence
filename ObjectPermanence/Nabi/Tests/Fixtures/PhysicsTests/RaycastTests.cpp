#include "TestCore.h"

#include "AABB.h"
#include "CoreModules\PhysicsModule.h"
#include "PhysicsTestHelpers.h"

#ifdef RUN_TESTS

namespace nabi::Tests::PhysicsTests
{
	ecs::ColliderComponent::ColliderMask constexpr c_ValidEntityMask   = 1 << 1;
	ecs::ColliderComponent::ColliderMask constexpr c_InvalidEntityMask = 1 << 2;

	// Check that a raycast hits an AABB correctly (with valid/invalid mask)
	TEST(PhysicsTests, CheckRaycastSingle)
	{
		// Mock core objects
		nabi::Context context;

		entt::entity const corePhysicsEntity = context.m_Registry.create();
		auto const& collisionStateComponent = context.m_Registry.emplace<ecs::SComp::CollisionStateComponent>(corePhysicsEntity);
		context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics) = corePhysicsEntity;

		// Create collision entity
		Helpers::TestCollisionEntitySettings entitySettings = Helpers::c_DefaultTestCollisionEntitySettings;
		entitySettings.m_Position = { 1.0f, 0.0f, 0.0f };
		entitySettings.m_Mask = c_ValidEntityMask;
		entt::entity const entity = Helpers::CreateTestCollisionEntity(context, entitySettings);
		
		// Create the ray settings
		ecs::PhysicsModule::RaycastSettings settings = ecs::PhysicsModule::c_DefaultRaycastSettings;
		settings.m_Mask = c_ValidEntityMask;

		dx::XMFLOAT3 constexpr origin = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr direction = { 1.0f, 0.0f, 0.0f };
		nabi::Physics::RaycastHitResult result = {};

		// Fire a ray and check there is a hit
		result = ecs::PhysicsModule::Raycast(context, origin, direction, settings);
		EXPECT_EQ(entity, result.m_Entity);

		// Change the entity's mask and check there is no hit
		context.m_Registry.get<ecs::ColliderComponent>(entity).m_Mask = c_InvalidEntityMask;
		result = ecs::PhysicsModule::Raycast(context, origin, direction, settings);

		Comparison<entt::entity> noHitComparison = {}; // gotta use a comparison here for entt::null
		noHitComparison.m_Expected = entt::null;
		noHitComparison.m_Actual = result.m_Entity;
		COMPAIR_EQ(noHitComparison);
	}

	// Check that a raycast hits AABBs correctly
	TEST(PhysicsTests, CheckRaycastMultiple)
	{
		// Mock core objects
		nabi::Context context;

		entt::entity const corePhysicsEntity = context.m_Registry.create();
		auto const& collisionStateComponent = context.m_Registry.emplace<ecs::SComp::CollisionStateComponent>(corePhysicsEntity);
		context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics) = corePhysicsEntity;

		// Create collision entities
		Helpers::TestCollisionEntitySettings entityOneSettings = Helpers::c_DefaultTestCollisionEntitySettings;
		entityOneSettings.m_Position = { 1.0f, 0.0f, 0.0f };
		entityOneSettings.m_Mask = c_ValidEntityMask;
		entt::entity const entityOne = Helpers::CreateTestCollisionEntity(context, entityOneSettings);

		Helpers::TestCollisionEntitySettings entityTwoSettings = Helpers::c_DefaultTestCollisionEntitySettings;
		entityTwoSettings.m_Position = { 2.0f, 0.0f, 0.0f };
		entityTwoSettings.m_Mask = c_ValidEntityMask;
		entt::entity const entityTwo = Helpers::CreateTestCollisionEntity(context, entityTwoSettings);

		// Fire a ray
		ecs::PhysicsModule::RaycastSettings settings = ecs::PhysicsModule::c_DefaultRaycastSettings;
		settings.m_Mask = c_ValidEntityMask;

		dx::XMFLOAT3 constexpr origin = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr direction = { 1.0f, 0.0f, 0.0f };

		int constexpr numberOfResults = 2;
		std::vector<nabi::Physics::RaycastHitResult> const results = ecs::PhysicsModule::Raycast(context, origin, direction, numberOfResults, settings);

		EXPECT_EQ(2u, results.size());

		if (results.size() == 2u)
		{
			EXPECT_EQ(results[0].m_Entity, entityOne);
			EXPECT_EQ(results[1].m_Entity, entityTwo);
		}
	}
} // namespace nabi::Tests::PhysicsTests

#endif // #ifdef RUN_TESTS
