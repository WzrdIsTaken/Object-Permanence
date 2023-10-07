#include "EngineCore.h"

#include "AudioEffect.h"

#include "WinIncludes.h" // For context include
#include "AudioIncludes.h"

#include "Context.h"

namespace nabi::Audio
{
	AudioEffectLoader::ResourceType AudioEffectLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ResourceType result = std::make_shared<nabi::Audio::AudioEffect>();
		context.m_AudioCommand->LoadAudioEffect(*result, resourcePath, m_LoadSettings);

		return result;
	}

	void AudioEffectLoader::SetLoadSettings(AudioCommand::LoadSettings const& loadSettings) NABI_NOEXCEPT
	{
		m_LoadSettings = loadSettings;
	}
} // namespace nabi::Audio
