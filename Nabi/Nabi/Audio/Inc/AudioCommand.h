#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "DirectXIncludes.h"
#include "AudioIncludes.h"

#include "TypeUtils.h"
#include "WorldConstants.h"
#include "XAudioObjects.h"

// Super useful resource for this - https://stackoverflow.com/questions/67418892/playing-audio-with-xaudio2-c

namespace nabi::Audio
{
	class AudioSourceVoice;
	struct AudioEffect;
} // namespace nabi::Audio

namespace nabi::Audio
{
	/// <summary>
	/// Stored in the Context, AudioCommand manages the XAudioObjects as well as loading/playing/deleting audio effects and voices.
	/// The 3d audio listener can also be set through AudioCommand. Apart from this, you shouldn't need to interface with AudioCommand directly, 
	/// instead there are a number of helper methods in the AudioModule.
	/// </summary>
	class AudioCommand final
	{
	public:
		struct LoadSettings final
		{
			bool m_Loop;
		};
		static LoadSettings constexpr c_DefaultLoadSettings = 
		{
			.m_Loop = false
		};

		AudioCommand(XAudioObjects& xaudioObjects) NABI_NOEXCEPT;
		~AudioCommand() NABI_NOEXCEPT;

		void LoadAudioEffect(AudioEffect& audioEffect, std::string const& filepath, LoadSettings const& loadSettings) const NABI_NOEXCEPT;
		void LoadAudioVoice(AudioEffect& audioEffect, AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT;

		void DestroyAudioEffect(AudioEffect& audioEffect) const NABI_NOEXCEPT;
		void DestroyAudioVoice(AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT;

		void SetListener(dx::XMFLOAT3 const& position, dx::XMFLOAT3 const& rotation, dx::XMFLOAT3 const& velocity,
			dx::XMFLOAT3 const& worldForward = WorldConstants::c_Forward, dx::XMFLOAT3 const& worldUp = WorldConstants::c_Up) const NABI_NOEXCEPT;
		void Calculate3DSoundProperties(X3DAUDIO_EMITTER const& audioEmitter, AudioSourceVoice& audioSourceVoice, X3DAUDIO_DSP_SETTINGS& audioDspSettings) const NABI_NOEXCEPT;

	    /*static constexpr?*/ inline UINT GetSourceChannelCount() const NABI_NOEXCEPT { return c_SourceChannelCount; }
		inline UINT GetInputChannelCount() const NABI_NOEXCEPT { return m_XAudioObjects.m_MasteringVoiceDetails.InputChannels; }

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(AudioCommand)

		static DWORD constexpr c_FourccRIFF = 'FFIR';
		static DWORD constexpr c_FourccDATA = 'atad';
		static DWORD constexpr c_FourccFMT  = ' tmf';
		static DWORD constexpr c_FourccWAVE = 'EVAW';
		static DWORD constexpr c_FourccXWMA = 'AMWX';
		static DWORD constexpr c_FourccDPDS = 'sdpd';
		static UINT  constexpr c_SourceChannelCount = 2u; // .wav has 2 channels

		[[nodiscard]] HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) const NABI_NOEXCEPT;
		[[nodiscard]] HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) const NABI_NOEXCEPT;

		XAudioObjects& m_XAudioObjects;
	};
} // namespace nabi::Audio
