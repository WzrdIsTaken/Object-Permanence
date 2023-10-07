#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "CoreComponents\RigidbodyComponent.h" // Can't forward declare nested classes ):
#include "SystemBase.h"
#include "WorldConstants.h"

namespace ecs
{
	struct TransformComponent;
} // namespace ecs
namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi

namespace ecs
{
	class PhysicsSystem final : public nabi::ECS::SystemBase
	{
	public:
		PhysicsSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~PhysicsSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		// --- Transform Operations ---

		void SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const;
		void SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const;

		void SetTransformValues(dx::XMFLOAT3& values, dx::XMFLOAT3 const& newValues, RigidbodyComponent::Constraints const& valueConstraints) const;

		// --- Rigidbody Operations ---

		void ComputeLinearVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;
		void ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;

		[[nodiscard]] dx::XMFLOAT3 ComputeForce(RigidbodyComponent const& rigidbodyComponent) const;
		[[nodiscard]] dx::XMFLOAT3 ComputeAcceleration(RigidbodyComponent const& rigidbodyComponent, dx::XMFLOAT3 const& force) const;

		void AccountForDrag(dx::XMFLOAT3& velocity, float const drag, float const dt) const;

		static dx::XMFLOAT3 constexpr c_Gravity = nabi::WorldConstants::c_Gravity;

		REFLECT_PRIVATES(PhysicsSystem)
	};
} // namespace ecs