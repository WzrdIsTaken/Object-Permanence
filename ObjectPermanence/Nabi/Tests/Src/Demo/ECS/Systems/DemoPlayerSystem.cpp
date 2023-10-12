#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Systems\PlayerSystem.h"

#include "Context.h"
#include "CoreComponents\EntityInfoComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\InputModule.h"
#include "CoreModules\CameraModule.h"
#include "CoreModules\PhysicsModule.h"
#include "DirectXUtils.h"
#include "GameTime.h"

#include "Demo\ECS\Components\PlayerComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(PlayerSystem)
	RELFECT_SYSTEM_END(PlayerSystem)

	PlayerSystem::PlayerSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PlayerSystem)
	}

	PlayerSystem::~PlayerSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PlayerSystem)
	}

	void PlayerSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime());

		m_Context.m_Registry.view<PlayerComponent const, RigidbodyComponent, TransformComponent const>()
			.each([&](auto& playerComponent, auto& rigidbodyComponent, auto& transformComponent) -> void
				{
					MovePlayer(dt, playerComponent, rigidbodyComponent);
					RotatePlayer(dt, playerComponent, rigidbodyComponent);
					RaycastFromPlayer(playerComponent, transformComponent);

					m_Context.m_AudioCommand->SetListener(
						transformComponent.m_Position, transformComponent.m_Rotation, rigidbodyComponent.m_Velocity);
					
					CameraModule::UpdateCamera(m_Context, CameraModule::GetMainPerspectiveCameraComponent(m_Context),
						[transformComponent](CameraComponent& cameraComponent) -> void
						{
							cameraComponent.m_Position = transformComponent.m_Position;
							cameraComponent.m_Rotation = transformComponent.m_Rotation;
						});
				});
	}

	void PlayerSystem::MovePlayer(float const dt, PlayerComponent const& playerComponent, RigidbodyComponent& rigidbodyComponent) const
	{
		using namespace nabi::Input;
		float const moveSpeed = playerComponent.m_RotateSpeed * dt;

		auto const fwdKeyState   = InputModule::GetKeyboardKey(m_Context, playerComponent.m_ForwardKey);
		auto const bwdKeyState   = InputModule::GetKeyboardKey(m_Context, playerComponent.m_BackwardKey);
		auto const leftKeyState  = InputModule::GetKeyboardKey(m_Context, playerComponent.m_LeftKey);
		auto const rightKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RightKey);
		auto const upKeyState    = InputModule::GetKeyboardKey(m_Context, playerComponent.m_UpKey);
		auto const downKeyState  = InputModule::GetKeyboardKey(m_Context, playerComponent.m_DownKey);

		if (fwdKeyState   == InputState::Held) rigidbodyComponent.m_Velocity.z -= moveSpeed;
		if (bwdKeyState   == InputState::Held) rigidbodyComponent.m_Velocity.z += moveSpeed;
		if (leftKeyState  == InputState::Held) rigidbodyComponent.m_Velocity.x -= moveSpeed;
		if (rightKeyState == InputState::Held) rigidbodyComponent.m_Velocity.x += moveSpeed;
		if (upKeyState    == InputState::Held) rigidbodyComponent.m_Velocity.y -= moveSpeed;
		if (downKeyState  == InputState::Held) rigidbodyComponent.m_Velocity.y += moveSpeed;
	}

	void PlayerSystem::RotatePlayer(float const dt, PlayerComponent const& playerComponent, RigidbodyComponent& rigidbodyComponent) const
	{
		using namespace nabi::Input;
		float const rotationSpeed = playerComponent.m_RotateSpeed * dt;

		auto const fwdKeyState   = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RotateForwardKey);
		auto const bwdKeyState   = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RotateBackwardKey);
		auto const leftKeyState  = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RotateLeftKey);
		auto const rightKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RotateRightKey);

		if (fwdKeyState   == InputState::Held) rigidbodyComponent.m_AngularVelocity.x += rotationSpeed;
		if (bwdKeyState   == InputState::Held) rigidbodyComponent.m_AngularVelocity.x -= rotationSpeed;
		if (leftKeyState  == InputState::Held) rigidbodyComponent.m_AngularVelocity.y += rotationSpeed;
		if (rightKeyState == InputState::Held) rigidbodyComponent.m_AngularVelocity.y -= rotationSpeed;
	}

	void PlayerSystem::RaycastFromPlayer(PlayerComponent const& playerComponent, TransformComponent const& transformComponent) const
	{
		using namespace nabi::Input;
		auto constexpr raycastDirection = nabi::WorldConstants::c_Forward;

		auto const raycastKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RaycastKey);
		if (raycastKeyState == InputState::Pressed)
		{
			float constexpr smallDistanceInFrontOfPlayer = 2.0f;
			dx::XMFLOAT3 raycastFrom = nabi::DirectXUtils::Float3Multiply(raycastDirection, smallDistanceInFrontOfPlayer);
			raycastFrom = nabi::DirectXUtils::Float3Add(transformComponent.m_Position, raycastFrom);

			auto const result = PhysicsModule::Raycast(
				m_Context,
				raycastFrom,
				raycastDirection,
				ecs::PhysicsModule::c_DefaultRaycastSettings
			);

			if (result)
			{
				auto const& entityInfoComponent = m_Context.m_Registry.get<EntityInfoComponent>(result.m_Entity);
				float const distance = result.m_Distance;

				LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_DEMO, "Raycast hit " << entityInfoComponent.m_EntityName.data() << 
					" at " << distance << " units", LOG_END);
			}
			else
			{
				LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_DEMO, "Raycast didn't hit", LOG_END);
			}
		}
	}
} // namespace ecs

#endif INCLUDE_DEMO
