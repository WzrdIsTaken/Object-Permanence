#pragma once
#include "TestCore.h"

#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"

#ifdef RUN_TESTS

namespace nabi::Tests::PhysicsTests::Helpers
{
	// Used in CollisionTests + RaycastTests

	struct TestCollisionEntitySettings final
	{
		dx::XMFLOAT3 m_Position;
		ecs::ColliderComponent::ColliderMask m_Mask;
	};
	TestCollisionEntitySettings constexpr c_DefaultTestCollisionEntitySettings =
	{
		.m_Position = { 0.0f, 0.0f, 0.0f },
		.m_Mask = 1 << 1
	};

	static entt::entity CreateTestCollisionEntity(nabi::Context& context, TestCollisionEntitySettings const& settings)
	{
		entt::entity entity = context.m_Registry.create();

		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = settings.m_Position;
		transformComponent.m_Rotation = { 0.0f, 0.0f, 0.0f };
		transformComponent.m_Scale = { 0.0f, 0.0f, 0.0f };

		ecs::RigidbodyComponent rigidbodyComponent = {};

		ecs::ColliderComponent colliderComponent = {};
		colliderComponent.m_ColliderType = ecs::ColliderComponent::ColliderType::Cube;
		colliderComponent.m_InteractionType = ecs::ColliderComponent::InteractionType::Dynamic;
		colliderComponent.m_ColliderDimensions = { 0.625f, 0.625f, 0.625f };
		colliderComponent.m_Mask = settings.m_Mask;

		context.m_Registry.emplace<ecs::TransformComponent>(entity, transformComponent);
		context.m_Registry.emplace<ecs::RigidbodyComponent>(entity, rigidbodyComponent);
		context.m_Registry.emplace<ecs::ColliderComponent> (entity, colliderComponent );

		return entity;
	}
} // namespace nabi::Tests::PhysicsTests::Helpers

#endif // #ifdef RUN_TESTS
