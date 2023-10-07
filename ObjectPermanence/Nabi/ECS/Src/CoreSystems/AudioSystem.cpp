#include "Core.h"

#include "CoreSystems\AudioSystem.h"

#include "Context.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DirectXUtils.h"
#include "GameTime.h"
#include "WorldConstants.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(AudioSystem)
	RELFECT_SYSTEM_END(AudioSystem)

	AudioSystem::AudioSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AudioSystem)
		m_Context.m_Registry.on_construct<AudioEmitterComponent>().connect<&AudioSystem::OnAudioEmitterCreated>(this);
	}

	AudioSystem::~AudioSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AudioSystem)
		m_Context.m_Registry.on_construct<AudioEmitterComponent>().disconnect<&AudioSystem::OnAudioEmitterCreated>(this);
	}

	void AudioSystem::Update(nabi::GameTime const& /*gameTime*/)
	{
		m_Context.m_Registry.view<TransformComponent const, AudioEmitterComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto& audioEmitterComponent) -> void
				{
					// Update the emitter's position and orientation
					dx::XMFLOAT3 const emitterForward = nabi::DirectXUtils::Float3Normalize(
						nabi::DirectXUtils::Float3Rotate(transformComponent.m_Rotation, nabi::WorldConstants::c_Forward));
					dx::XMFLOAT3 const emitterUp = nabi::DirectXUtils::Float3Normalize(
						nabi::DirectXUtils::Float3Rotate(transformComponent.m_Rotation, nabi::WorldConstants::c_Up));

					X3DAUDIO_EMITTER& emitter = audioEmitterComponent.m_Emitter;
					emitter.OrientFront.x = emitterForward.x;
					emitter.OrientFront.y = emitterForward.y;
					emitter.OrientFront.z = emitterForward.z;
					emitter.OrientTop.x   = emitterUp.x;
					emitter.OrientTop.y   = emitterUp.y;
					emitter.OrientTop.z   = emitterUp.z;
					emitter.Position.x    = transformComponent.m_Position.x;
					emitter.Position.y    = transformComponent.m_Position.y;
					emitter.Position.z    = transformComponent.m_Position.z;

					// Check if the emitter entity has a rigidbody component, and if so update its velocity
					if (m_Context.m_Registry.any_of<RigidbodyComponent>(entity))
					{
						dx::XMFLOAT3 const& velocity = m_Context.m_Registry.get<RigidbodyComponent>(entity).m_Velocity;
						emitter.Velocity.x = velocity.x;
						emitter.Velocity.y = velocity.y;
						emitter.Velocity.z = velocity.z;
					}
					else
					{
						emitter.Velocity = X3DAUDIO_VECTOR(0.0f, 0.0f, 0.0f);
					}
				});
	}

	void AudioSystem::OnAudioEmitterCreated(entt::registry& /*registry*/, entt::entity const entity) const
	{
		auto& audioEmitterComponent = m_Context.m_Registry.get<AudioEmitterComponent>(entity);
		UINT const sourceChannelCount = m_Context.m_AudioCommand->GetSourceChannelCount();
		UINT const inputChannelCount = m_Context.m_AudioCommand->GetInputChannelCount();

		X3DAUDIO_EMITTER& emitter = audioEmitterComponent.m_Emitter;
		X3DAUDIO_DSP_SETTINGS& dpsSettings = audioEmitterComponent.m_DSPSettings;
		AudioEmitterComponent::DSPMatrix& dpsMatrix = audioEmitterComponent.m_DSPMatrix;

		dpsMatrix.resize(sourceChannelCount * inputChannelCount);
		dpsSettings.SrcChannelCount = sourceChannelCount;
		dpsSettings.DstChannelCount = inputChannelCount;
		dpsSettings.pMatrixCoefficients = dpsMatrix.data();

		emitter.ChannelCount = 1u;
		emitter.CurveDistanceScaler = 1.0f;
	}
} // namespace ecs
