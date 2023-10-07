#include "TestCore.h"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSystems\PhysicsSystem.h"
#include "DirectXUtils.h"
#include "GameTime.h"

#ifdef RUN_TESTS

namespace nabi::Tests::PhysicsTests
{
	// Check that rigidbodies move when force is applied to them
	TEST(PhysicsTests, CheckRigidbodies)
	{
		// Mock objects
		nabi::Context context;
		ecs::PhysicsSystem physicsSystem(context, "id"_hs, "group"_hs);

		nabi::GameTime gameTime = {};
		gameTime.Tick();

		entt::entity const rbEntity = context.m_Registry.create();
		auto& transformComponent = context.m_Registry.emplace<ecs::TransformComponent>(rbEntity);
		auto& rbComponent = context.m_Registry.emplace<ecs::RigidbodyComponent>(rbEntity);
		
		// With a gravity scale of 0, check that the entity doesn't move
		rbComponent.m_GravityScale = 0.0f;
		Comparison<dx::XMFLOAT3> positionComparison(transformComponent.m_Position);

		physicsSystem.Update(gameTime); physicsSystem.Update(gameTime); // physics system updates pos then velo, so 2 ticks are needed
		positionComparison.m_Actual = transformComponent.m_Position;

		bool result = nabi::DirectXUtils::Float3Equal(positionComparison.m_Expected, positionComparison.m_Actual);
		EXPECT_TRUE(result);

		// With a gravity scale of 1, the entity should move down
		rbComponent.m_GravityScale = 1.0f;
		physicsSystem.Update(gameTime); physicsSystem.Update(gameTime);

		result = positionComparison.m_Expected.y > transformComponent.m_Position.y;
		EXPECT_TRUE(result);

		// Apply a force moving the rb to the right
		rbComponent.m_Velocity = { 10.0f, 0.0f, 0.0f };
		physicsSystem.Update(gameTime); physicsSystem.Update(gameTime);

		result = positionComparison.m_Expected.x < transformComponent.m_Position.x;
		EXPECT_TRUE(result);
	}
} // namespace nabi::Tests::PhysicsTests

#endif // #ifdef RUN_TESTS
