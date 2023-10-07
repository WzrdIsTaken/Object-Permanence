#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "DirectXIncludes.h"
#include "AudioIncludes.h"

namespace nabi::Audio
{
	/// <summary>
	/// Holds all of the xaudio objects needed for 2d and 3d sound.
	/// These objects are stored in NabiCore, and used in AudioCommand.
	/// </summary>

	struct XAudioObjects final
	{
		wrl::ComPtr<IXAudio2> m_XAudio2;
		IXAudio2MasteringVoice* m_MasteringVoice;

		X3DAUDIO_HANDLE m_X3DAudio;
		X3DAUDIO_LISTENER m_Listener;
		XAUDIO2_VOICE_DETAILS m_MasteringVoiceDetails;

#ifdef USE_DEBUG_UTILS
		XAUDIO2_DEBUG_CONFIGURATION m_DebugConfiguration;
#endif // ifdef USE_DEBUG_UTILS
	};
	
	XAudioObjects const c_XAudioObjectsDefaultSettings
	{
		.m_XAudio2 = nullptr,
		.m_MasteringVoice = nullptr,
		.m_X3DAudio = NULL,
		.m_Listener = {},
		.m_MasteringVoiceDetails = {}

#ifdef USE_DEBUG_UTILS
		, .m_DebugConfiguration{}
#endif // ifdef USE_DEBUG_UTILS
	};
} // namespace nabi::Audio
