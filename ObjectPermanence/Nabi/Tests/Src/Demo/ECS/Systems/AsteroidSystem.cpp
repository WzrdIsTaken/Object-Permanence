#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Systems\AsteroidSystem.h"

#include "Context.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\EntityModule.h"
#include "CoreModules\InputModule.h"
#include "DirectXUtils.h"
#include "GameTime.h"

#include "Demo\Core\Demo.h"
#include "Demo\Core\DemoEnums.h"
#include "Demo\ECS\Components\AsteroidComponent.h"
#include "Demo\ECS\SingletonComponents\DemoPropertiesComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(AsteroidSystem)
	RELFECT_SYSTEM_END(AsteroidSystem)

	AsteroidSystem::AsteroidSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
		, m_LastAudioEmitter(nullptr)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AsteroidSystem)
	}

	AsteroidSystem::~AsteroidSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AsteroidSystem)
	}

	void AsteroidSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime());

		MoveAndRotateAsteroids(dt);
		CheckInput();
	}

	void AsteroidSystem::MoveAndRotateAsteroids(float const dt) const
	{
		m_Context.m_Registry.view<TransformComponent, AsteroidComponent const, AudioEmitterComponent>().each(
			[&](auto& transformComponent, auto& asteroidComponent, auto& audioEmitterComponent) -> void
			{
				dx::XMFLOAT3 moveSpeed = nabi::DirectXUtils::Float3Multiply(asteroidComponent.m_MoveDirection, asteroidComponent.m_MoveSpeed);
				moveSpeed = nabi::DirectXUtils::Float3Multiply(moveSpeed, dt);

				dx::XMFLOAT3 rotationSpeed = nabi::DirectXUtils::Float3Multiply(asteroidComponent.m_SpinDirection, asteroidComponent.m_SpinSpeed);
				rotationSpeed = nabi::DirectXUtils::Float3Multiply(rotationSpeed, dt);

				transformComponent.m_Position = nabi::DirectXUtils::Float3Add(transformComponent.m_Position, moveSpeed);
				transformComponent.m_Rotation = nabi::DirectXUtils::Float3Add(transformComponent.m_Rotation, rotationSpeed);

				// ---

				m_LastAudioEmitter = &audioEmitterComponent;
			});
	}

	void AsteroidSystem::CheckInput() const
	{
		auto const& demoPropertiesComponent = EntityModule::GetSingletonComponent<SComp::DemoPropertiesComponent>(m_Context);

		// --

		auto const loadAsteroidGroupKeyState = InputModule::GetKeyboardKey(m_Context, demoPropertiesComponent.m_LoadAsteroidGroupKey);
		auto const unloadAsteroidGroupKeyState = InputModule::GetKeyboardKey(m_Context, demoPropertiesComponent.m_UnloadAsteroidGroupKey);

		if (loadAsteroidGroupKeyState == nabi::Input::InputState::Pressed)
		{
			m_Context.m_EntityCreator->CreateEntityGroup(c_AsteroidGroupName.data());
			nabi::ContextHelpers::CorePointerOperation<core::Demo>(m_Context,
				[](auto* const corePointer) -> void
				{
					corePointer->RefreshLoadedAssets(core::AssetType::Model); // See Demo.cpp::RefreshLoadedAssets for an explanation
				});
		}
		if (unloadAsteroidGroupKeyState == nabi::Input::InputState::Pressed)
		{
			m_Context.m_EntityCreator->DestroyEntityGroup(c_AsteroidGroupName.data());
		}

		// ---

		auto const playSoundEffectKey = InputModule::GetKeyboardKey(m_Context, demoPropertiesComponent.m_PlaySoundEffectKey);

		if (playSoundEffectKey == nabi::Input::InputState::Pressed)
		{
			PlaySoundEffect();
		}
	}

	void AsteroidSystem::PlaySoundEffect() const
	{
		auto constexpr audioID = static_cast<SComp::AudioStateComponent::AudioID>(core::AudioID::BingoBangoBongo);

		// 2D
		//AudioModule::Play2DAudioEffect(m_Context, audioID, AudioModule::c_DefaultPlaySettings);

		// 3D
		if (m_LastAudioEmitter)
		{
			AudioModule::Play3DAudioEffect(m_Context, *m_LastAudioEmitter, audioID, AudioModule::c_DefaultPlaySettings);
		}
	}
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
