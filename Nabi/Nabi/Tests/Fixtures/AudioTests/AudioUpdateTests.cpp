#include "TestCore.h"

#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSystems\AudioSystem.h"
#include "DirectXUtils.h"

#ifdef RUN_TESTS

namespace nabi::Tests::AudioTests
{
	// Check that the audio system updates audio emitters correctly
	TEST(AudioTests, CheckAudioEmitterRespondsToTransformAndRigidbodyMovement)
	{
#ifdef RUN_SLOW_TESTS
		// Mock objects
		nabi::Context context;
		nabi::Audio::XAudioObjects xaudioObjects;
		nabi::GameTime gameTime;

		context.m_Registry = {};
		context.m_AudioCommand = std::make_unique<nabi::Audio::AudioCommand>(xaudioObjects);

		ecs::AudioSystem audioSystem{ context, ""_hs, ""_hs };

		// Create a audio emitter and transform component
		entt::entity const entity = context.m_Registry.create();
		auto const& audioEmitterComponent = context.m_Registry.emplace<ecs::AudioEmitterComponent>(entity);
		auto& transformComponent = context.m_Registry.emplace<ecs::TransformComponent>(entity);
		auto& rigidbodyComponent = context.m_Registry.emplace<ecs::RigidbodyComponent>(entity);

		auto const& audioEmitter = audioEmitterComponent.m_Emitter;

		// Move the transform component
		dx::XMFLOAT3 constexpr movePosition = { 17.0f, 6.0f, 23.0f };
		transformComponent.m_Position = movePosition;

		// Update the audio system 
		audioSystem.Update(gameTime);

		// Check that the audio emitter component's position has been updated
		dx::XMFLOAT3 const emitterPosition = {
			audioEmitter.Position.x,
			audioEmitter.Position.y,
			audioEmitter.Position.z
		};
		EXPECT_TRUE(nabi::DirectXUtils::Float3Equal(movePosition, emitterPosition));

		// Do the same test, but with velocity
		dx::XMFLOAT3 constexpr velocityChange = { 20.0f, 7.0f, 23.0f };
		rigidbodyComponent.m_Velocity = velocityChange;

		audioSystem.Update(gameTime);

		dx::XMFLOAT3 const emitterVelocity = {
			audioEmitter.Velocity.x,
			audioEmitter.Velocity.y,
			audioEmitter.Velocity.z
		};
		EXPECT_TRUE(nabi::DirectXUtils::Float3Equal(velocityChange, emitterVelocity));

		// Not quite sure why, but if we don't do this we get a crash in AudioCommand's destructor
		context.m_AudioCommand.reset();
#else
		LOG_NOT_RUNNING_SLOW_TEST_WARNING
#endif // #ifdef RUN_SLOW_TESTS
	}
} // namespace nabi::Tests::AudioTests

#endif // #ifdef RUN_TESTS
