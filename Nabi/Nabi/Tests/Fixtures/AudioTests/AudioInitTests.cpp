#include "TestCore.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreSystems\AudioSystem.h"

#ifdef RUN_TESTS

namespace nabi::Tests::AudioTests
{
	// Check that audio effects and voices are loaded and deleted ok
	TEST(AudioTests, LoadAudioEffectAndVoiceThenPlay)
	{
#ifdef RUN_SLOW_TESTS
		nabi::Audio::XAudioObjects xaudioObjects;
		nabi::Audio::AudioCommand audioCommand{ xaudioObjects };

		nabi::Audio::AudioEffect audioEffect;
		std::string const filePath = "Nabi/Tests/Data/Audio/bingo_bango_bongo_bish_bash_bosh.wav";
		auto constexpr loadSettings = nabi::Audio::AudioCommand::c_DefaultLoadSettings;
		audioCommand.LoadAudioEffect(audioEffect, filePath, loadSettings);

		nabi::Audio::AudioSourceVoice audioSourceVoice;
		audioCommand.LoadAudioVoice(audioEffect, audioSourceVoice);

		auto* const sourceVoice = audioSourceVoice.GetSourceVoice();
		sourceVoice->SetVolume(0.0f);
		sourceVoice->Start();

		audioCommand.DestroyAudioEffect(audioEffect);
		audioCommand.DestroyAudioVoice(audioSourceVoice);

		// There would be an assert somewhere in here if this test failed
#else
		LOG_NOT_RUNNING_SLOW_TEST_WARNING
#endif // #ifdef RUN_SLOW_TESTS
	}

	// Check that audio emitters have 2 source channels, required for playing .wav
	TEST(AudioTests, CheckAudioEmitterConfigureCorrectly)
	{
#ifdef RUN_SLOW_TESTS
		// Mock objects
		nabi::Context context;
		nabi::Audio::XAudioObjects xaudioObjects;

		context.m_Registry = {};
		context.m_AudioCommand = std::make_unique<nabi::Audio::AudioCommand>(xaudioObjects);

		ecs::AudioSystem audioSystem{ context, ""_hs, ""_hs };

		// Create a audio emitter component and check it has 2 source channels (this is set in AudioSystem::OnAudioEmitterCreated)
		entt::entity const entity = context.m_Registry.create();
		auto const& audioEmitterComponent = context.m_Registry.emplace<ecs::AudioEmitterComponent>(entity);
		EXPECT_EQ(2u, audioEmitterComponent.m_DSPSettings.SrcChannelCount);

		// Not quite sure why, but if we don't do this we get a crash in AudioCommand's destructor
		context.m_AudioCommand.reset();
#else
		LOG_NOT_RUNNING_SLOW_TEST_WARNING
#endif // #ifdef RUN_SLOW_TESTS
	}
} // namespace nabi::Tests::AudioTests

#endif // #ifdef RUN_TESTS
