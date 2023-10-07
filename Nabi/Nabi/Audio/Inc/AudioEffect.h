#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "AudioIncludes.h"

#include "AudioCommand.h"

namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::Audio
{
	/// <summary>
	/// The loaded data of an audio effect. Nabi currently only supports .wav files.
	/// </summary>
	struct AudioEffect final
	{
		std::shared_ptr<BYTE[]> m_DataBuffer;
		WAVEFORMATEXTENSIBLE m_WFX;
		XAUDIO2_BUFFER m_Buffer;

		AudioEffect() NABI_NOEXCEPT
			: m_DataBuffer{ nullptr }
			, m_WFX{}
			, m_Buffer{}
		{
		}
	};

	/// <summary>
	/// Loads an audio effect. Set m_LoadSettings to control how the effect is loaded. 
	/// </summary>
	class AudioEffectLoader final
	{
	public:
		typedef std::shared_ptr<AudioEffect> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		void SetLoadSettings(AudioCommand::LoadSettings const& loadSettings) NABI_NOEXCEPT;

	private:
		AudioCommand::LoadSettings m_LoadSettings = AudioCommand::c_DefaultLoadSettings;
	};
} // namespace nabi::Audio
