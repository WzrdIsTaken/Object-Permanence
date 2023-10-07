#include "Core.h"

#include "CoreSystems\PhysicsSystem.h"

#include "Context.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DirectXUtils.h"
#include "GameTime.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(PhysicsSystem)
	RELFECT_SYSTEM_END(PhysicsSystem)

	PhysicsSystem::PhysicsSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	PhysicsSystem::~PhysicsSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	void PhysicsSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime()); // Perhaps this should use fixed dt? But when I used that things seemed less smooth

		m_Context.m_Registry.view<TransformComponent, RigidbodyComponent>()
			.each([&](auto& transformComponent, auto& rigidbodyComponent) -> void
				{
					SetPosition(transformComponent, rigidbodyComponent, dt);
					SetRotation(transformComponent, rigidbodyComponent, dt);

					ComputeLinearVelocity(rigidbodyComponent, dt);
					ComputeAngularVelocity(rigidbodyComponent, dt);
				});
	}

	// ---

	void PhysicsSystem::SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const
	{
		dx::XMFLOAT3 const velocityTimesDt = nabi::DirectXUtils::Float3Multiply(rigidbodyComponent.m_Velocity, dt);
		dx::XMFLOAT3 const newPosition = nabi::DirectXUtils::Float3Add(transformComponent.m_Position, velocityTimesDt);

		SetTransformValues(transformComponent.m_Position, newPosition, rigidbodyComponent.m_PositionConstraints);
	}

	void PhysicsSystem::SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const
	{
		dx::XMFLOAT3 const angularVelocityTimesDt = nabi::DirectXUtils::Float3Multiply(rigidbodyComponent.m_AngularVelocity, dt);
		dx::XMFLOAT3 const newRotation = nabi::DirectXUtils::Float3Add(transformComponent.m_Rotation, angularVelocityTimesDt);

		SetTransformValues(transformComponent.m_Rotation, newRotation, rigidbodyComponent.m_RotationConstraints);
	}

	void PhysicsSystem::SetTransformValues(dx::XMFLOAT3& values, dx::XMFLOAT3 const& newValues, RigidbodyComponent::Constraints const& valueConstraints) const
	{
		if (!valueConstraints.m_FreezeX) values.x = newValues.x;
		if (!valueConstraints.m_FreezeY) values.y = newValues.y;
		if (!valueConstraints.m_FreezeZ) values.z = newValues.z;
	}

	// ---

	void PhysicsSystem::ComputeLinearVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		dx::XMFLOAT3 const force = ComputeForce(rigidbodyComponent);
		dx::XMFLOAT3 const acceleration = ComputeAcceleration(rigidbodyComponent, force);

		dx::XMFLOAT3 const accelerationTimesDt = nabi::DirectXUtils::Float3Multiply(acceleration, dt);
		dx::XMFLOAT3 newVelocity = nabi::DirectXUtils::Float3Add(rigidbodyComponent.m_Velocity, accelerationTimesDt);
		
		AccountForDrag(newVelocity, rigidbodyComponent.m_Drag, dt);
		rigidbodyComponent.m_Velocity = newVelocity;
	}

	void PhysicsSystem::ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		// I spent a while playing around with a more complex method of calculating angular velocity, but honestly there 
		// wasn't much different to just doing this. I think I need to be better at maths xD
		// I've shelved up the changes and may come back to it - but for now this is fine.

		dx::XMFLOAT3 const angularVelocityTimesDt = nabi::DirectXUtils::Float3Multiply(rigidbodyComponent.m_AngularVelocity, dt);
		dx::XMFLOAT3 newAngularVelocity = nabi::DirectXUtils::Float3Add(rigidbodyComponent.m_AngularVelocity, angularVelocityTimesDt);

		AccountForDrag(newAngularVelocity, rigidbodyComponent.m_AngularDrag, dt);
		rigidbodyComponent.m_AngularVelocity = newAngularVelocity;
	}

	dx::XMFLOAT3 PhysicsSystem::ComputeForce(RigidbodyComponent const& rigidbodyComponent) const
	{
		dx::XMFLOAT3 const gravityScale = nabi::DirectXUtils::Float3Multiply(c_Gravity, rigidbodyComponent.m_GravityScale);
		dx::XMFLOAT3 const gravityForce = nabi::DirectXUtils::Float3Multiply(gravityScale, rigidbodyComponent.m_Mass);

		return gravityForce;
	}

	dx::XMFLOAT3 PhysicsSystem::ComputeAcceleration(RigidbodyComponent const& rigidbodyComponent, dx::XMFLOAT3 const& force) const
	{
		dx::XMFLOAT3 const acceleration = nabi::DirectXUtils::Float3Divide(force, rigidbodyComponent.m_Mass);

		return acceleration;
	}

	void PhysicsSystem::AccountForDrag(dx::XMFLOAT3& velocity, float const drag, float const dt) const
	{
		float const dragTimesDt = 1.0f + (drag * dt);
		velocity = nabi::DirectXUtils::Float3Divide(velocity, dragTimesDt);
	}
} // namespace ecs

/*
	WIP PhysicsModule that was being used to help compute angular velocity
	I think I need to be better at maths to make this stuff work...

	Resource: https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics

	.hpp:

	namespace ecs::PhysicsModule
	{
		void UpdateRigidbodyShape(nabi::Context& context, entt::entity const rigidbodyEntity, RigidbodyComponent::Shape::ShapeType const* const newShape);
		void UpdateRigidbodyShape(nabi::Context& context, RigidbodyComponent& rigidbody, RigidbodyComponent::Shape::ShapeType const* const newShape);

		void CalculateSphereInertia(nabi::Context& context, RigidbodyComponent& rigidbody);
		void CalculateCubeInertia(nabi::Context& context, RigidbodyComponent& rigidbody);
	} // namespace ecs::PhysicsModule

	-----

	.cpp:

	namespace ecs::PhysicsModule
	{
		namespace
		{
			RigidbodyComponent* TryGetRigidbodyComponent(nabi::Context& context, entt::entity const rigidbodyEntity)
			{
				RigidbodyComponent* rigidbodyComponent = context.m_Registry.try_get<RigidbodyComponent>(rigidbodyEntity);
				ASSERT(rigidbodyComponent != nullptr, "Trying to do a rigidbody operation an entity which doesn't have a rigidbody component!");

				return rigidbodyComponent;
			}
		}

		void UpdateRigidbodyShape(nabi::Context& context, entt::entity const rigidbodyEntity, RigidbodyComponent::Shape::ShapeType const* const newShape)
		{
			if (RigidbodyComponent* const rigidbodyComponent = TryGetRigidbodyComponent(context, rigidbodyEntity))
			{
				UpdateRigidbodyShape(context, *rigidbodyComponent, newShape);
			}
		}

		void UpdateRigidbodyShape(nabi::Context& context, RigidbodyComponent& rigidbody, RigidbodyComponent::Shape::ShapeType const* const newShape)
		{
			typedef RigidbodyComponent::Shape::ShapeType Shape;

			Shape shape = Shape::Default;
			if (newShape != nullptr)
			{
				shape = *newShape;
			}

			switch (shape)
			{
				case Shape::Sphere:
					CalculateSphereInertia(context, rigidbody);
					break;
				case Shape::Cube:
					CalculateCubeInertia(context, rigidbody);
					break;
				default:
					ASSERT_FAIL("Using an unexpected RigidbodyComponent::Shape");
					break;
			}
		}

		void CalculateSphereInertia(nabi::Context& context, RigidbodyComponent& rigidbody)
		{
			ASSERT_CODE
			(
				using namespace nabi::Utils;

				float const width = rigidbody.m_Shape.m_Width;
				float const height = rigidbody.m_Shape.m_Height;
				bool const widthAndHeightEqual = MathUtils::Approximity(width, height);

				// See RigidbodyComponent's reflection for an explanation
				ASSERT(widthAndHeightEqual, "For a sphere it is expected width, height and depth will all be equal");
			)

			float const mass = rigidbody.m_Mass;
			float const radius = rigidbody.m_Shape.m_Width;

			float constexpr moment = 2.0f / 5.0f;
			rigidbody.m_Internal.m_MomentOfInertia = mass * (radius * radius) * moment;
		}

		void CalculateCubeInertia(nabi::Context& context, RigidbodyComponent& rigidbody)
		{
			float const mass = rigidbody.m_Mass;
			float const width = rigidbody.m_Shape.m_Width;
			float const height = rigidbody.m_Shape.m_Height;
			float const depth = rigidbody.m_Shape.m_Depth;

			float constexpr moment = 6.0f;
			rigidbody.m_Internal.m_MomentOfInertia = mass * (width * width + height * height + depth * depth) / moment;
		}
	} // namespace ecs::PhysicsModule
*/
