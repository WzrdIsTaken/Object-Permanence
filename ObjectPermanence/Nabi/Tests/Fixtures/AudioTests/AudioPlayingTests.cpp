#include "TestCore.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreSingletonComponents\AudioStateComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreSystems\AudioSystem.h"

#ifdef RUN_TESTS

namespace nabi::Tests::AudioTests
{
	// Check that the audio module flow works
	TEST(AudioTests, CheckAudioModuleFlow)
	{
#ifdef RUN_SLOW_TESTS
		// If you want to actually hear this in action / see the full flow, run / look at the TestAudio example
#else
		//LOG_NOT_RUNNING_SLOW_TEST_WARNING (its not doing anything regardless...)
#endif // #ifdef RUN_SLOW_TESTS
	}
} // namespace nabi::Tests::AudioTests

#endif // #ifdef RUN_TESTS
