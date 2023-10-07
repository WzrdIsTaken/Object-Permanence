#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "AudioIncludes.h"

#include "DebugUtils.h"

namespace nabi::Audio
{
	/// <summary>
	/// Wraps an IXAudio2SourceVoice. Voices are what actually 'play' sounds.
	/// </summary>
	class AudioSourceVoice final
	{
	public:
		AudioSourceVoice() NABI_NOEXCEPT
			: m_SourceVoice(nullptr)
		{
		}

		/// <summary>
		/// Submits a buffer to the source voice. Onces this is done the audio is ready to be played.
		/// </summary>
		/// <param name="buffer">- A audio buffer stored in an AudioEffect</param>
		/// <returns>The result of submitting the buffer</returns>
		inline HRESULT SubmitBuffer(XAUDIO2_BUFFER const& buffer) NABI_NOEXCEPT
		{
			ASSERT_FATAL(m_SourceVoice, "Submitting a buffer to m_SourceVoice but its null");
			return m_SourceVoice->SubmitSourceBuffer(&buffer);
		}

		/// <summary>
		/// Returns true if the source voice is ready to play an audio track. This can be because the voice is null or if it has no queued tracks.
		/// </summary>
		inline bool IsReady() const NABI_NOEXCEPT
		{
			bool ready = false;

			if (!m_SourceVoice)
			{
				// If the source voice is null, it means it hasn't been used yet or whatever it was pointing to has been deleted - so its ready to use!
				ready = true;
			}
			else
			{
				XAUDIO2_VOICE_STATE voiceState;
				m_SourceVoice->GetState(&voiceState);
				ready = (voiceState.BuffersQueued == 0u);
			}

			return ready;
		}

		[[nodiscard]] inline IXAudio2SourceVoice*  const GetSourceVoice() const  NABI_NOEXCEPT { return m_SourceVoice;  }
		[[nodiscard]] inline IXAudio2SourceVoice** const GetSourceVoiceAddress() NABI_NOEXCEPT { return &m_SourceVoice; }

	private:
		IXAudio2SourceVoice* m_SourceVoice; // Set in AudioCommand::CreateAudioVoice
	};
} // namespace nabi::Audio
