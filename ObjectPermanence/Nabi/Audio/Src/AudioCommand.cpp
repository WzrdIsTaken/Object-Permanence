#include "EngineCore.h"

#include "AudioCommand.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"
#include "DebugUtils.h"
#include "DirectXUtils.h"
#include "StringUtils.h"

#pragma comment(lib, "xaudio2.lib")

namespace nabi::Audio
{
    AudioCommand::AudioCommand(XAudioObjects& xaudioObjects) NABI_NOEXCEPT
        : m_XAudioObjects(xaudioObjects)
    {
        // Initialise XAudio2
        DX_ASSERT(XAudio2Create(
			&m_XAudioObjects.m_XAudio2, // XAudio
            0u,                         // Flags
            XAUDIO2_DEFAULT_PROCESSOR   // Processor
        ));
        DX_ASSERT(m_XAudioObjects.m_XAudio2->CreateMasteringVoice(&m_XAudioObjects.m_MasteringVoice));
        m_XAudioObjects.m_MasteringVoice->GetVoiceDetails(&m_XAudioObjects.m_MasteringVoiceDetails);

        // Initialise X3DAudio
        DWORD dwChannelMask;
        m_XAudioObjects.m_MasteringVoice->GetChannelMask(&dwChannelMask);
        DX_ASSERT(X3DAudioInitialize(
            dwChannelMask,             // Speaker channel mask
            X3DAUDIO_SPEED_OF_SOUND,   // Speed of sound
            m_XAudioObjects.m_X3DAudio // XAudio3D
        ));

		// Initialise XAudio debug
#ifdef USE_DEBUG_UTILS
		m_XAudioObjects.m_DebugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_INFO;
		//m_XAudioObjects.m_DebugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;

		m_XAudioObjects.m_XAudio2->SetDebugConfiguration(&m_XAudioObjects.m_DebugConfiguration, NULL);
#endif // ifdef USE_DEBUG_UTILS
    }

    AudioCommand::~AudioCommand() NABI_NOEXCEPT
    {
		ASSERT(m_XAudioObjects.m_MasteringVoice, "Trying to shut down xaudio but m_MasteringVoice was null. This may result in a bad application exit");
		if (m_XAudioObjects.m_MasteringVoice)
		{
			m_XAudioObjects.m_MasteringVoice->DestroyVoice(); 
			m_XAudioObjects.m_MasteringVoice = nullptr;
		}

		ASSERT(m_XAudioObjects.m_XAudio2, "Trying to shut down xaudio but m_XAudio2 was null. This may result in a bad application exit");
		if (m_XAudioObjects.m_XAudio2)
		{
			m_XAudioObjects.m_XAudio2->StopEngine();         
			m_XAudioObjects.m_XAudio2 = nullptr;
		}
    }

	void AudioCommand::LoadAudioEffect(AudioEffect& audioEffect, std::string const& filepath, LoadSettings const& loadSettings) const NABI_NOEXCEPT
	{
		// Load the riff
		HANDLE file = CreateFile(
			STRING_TO_LPCWSTR_INLINE(filepath), // File name
			GENERIC_READ,                       // Access type
			FILE_SHARE_READ,                    // Share mode
			NULL,                               // Security attributes
			OPEN_EXISTING,                      // Creation disposition
			0ul,                                // Flags
			NULL);                              // Template file

		DX_ASSERT(file);
		DX_ASSERT(SetFilePointer(file, 0l, NULL, FILE_BEGIN));

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		DX_ASSERT(FindChunk(file, c_FourccRIFF, dwChunkSize, dwChunkPosition));

		DWORD fileType;
		DX_ASSERT(ReadChunkData(file, &fileType, sizeof(DWORD), dwChunkPosition));
		ASSERT_FATAL(fileType == c_FourccWAVE, "Trying to load an audio file which is not .wav");

		DX_ASSERT(FindChunk(file, c_FourccFMT, dwChunkSize, dwChunkPosition));
		DX_ASSERT(ReadChunkData(file, &audioEffect.m_WFX, dwChunkSize, dwChunkPosition));

		DX_ASSERT(FindChunk(file, c_FourccDATA, dwChunkSize, dwChunkPosition));
		audioEffect.m_DataBuffer = std::make_shared<BYTE[]>(dwChunkSize);
		DX_ASSERT(ReadChunkData(file, audioEffect.m_DataBuffer.get(), dwChunkSize, dwChunkPosition));

		audioEffect.m_Buffer.AudioBytes = dwChunkSize;
		audioEffect.m_Buffer.pAudioData = audioEffect.m_DataBuffer.get();
		audioEffect.m_Buffer.Flags      = XAUDIO2_END_OF_STREAM;

		if (loadSettings.m_Loop)
		{
			audioEffect.m_Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_AUDIO, "Loaded an audio effect from path " << filepath, LOG_END);
	}

	void AudioCommand::LoadAudioVoice(AudioEffect& audioEffect, AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT
	{
		DWORD constexpr creationFlags = XAUDIO2_VOICE_USEFILTER; /*| any_more_flags*/

		DX_ASSERT(m_XAudioObjects.m_XAudio2->CreateSourceVoice(audioSourceVoice.GetSourceVoiceAddress(), reinterpret_cast<WAVEFORMATEX*>(&audioEffect.m_WFX), creationFlags));
		DX_ASSERT(audioSourceVoice.SubmitBuffer(audioEffect.m_Buffer));
	}

	void AudioCommand::DestroyAudioEffect(AudioEffect& audioEffect) const NABI_NOEXCEPT
	{
		audioEffect.m_DataBuffer.reset();
		ZeroMemory(&audioEffect.m_WFX, sizeof(audioEffect.m_WFX));
		ZeroMemory(&audioEffect.m_Buffer, sizeof(audioEffect.m_Buffer));
	}

	void AudioCommand::DestroyAudioVoice(AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT
	{
		IXAudio2SourceVoice* sourceVoice = audioSourceVoice.GetSourceVoice();
		if (sourceVoice)
		{
			DX_ASSERT(sourceVoice->Stop());
			DX_ASSERT(sourceVoice->FlushSourceBuffers());
			DX_ASSERT(sourceVoice->SetVolume(0.0f));
			sourceVoice->DestroyVoice();
			sourceVoice = nullptr;
		}	
	}

	void AudioCommand::SetListener(dx::XMFLOAT3 const& position, dx::XMFLOAT3 const& rotation, dx::XMFLOAT3 const& velocity, 
		dx::XMFLOAT3 const& worldForward, dx::XMFLOAT3 const& worldUp) const NABI_NOEXCEPT
	{
		dx::XMFLOAT3 const listenerForward = DirectXUtils::Float3Normalize(DirectXUtils::Float3Rotate(rotation, worldForward));
		dx::XMFLOAT3 const listenerUp = DirectXUtils::Float3Normalize(DirectXUtils::Float3Rotate(rotation, worldUp));

		X3DAUDIO_LISTENER& listener = m_XAudioObjects.m_Listener;
		listener.OrientFront.x = listenerForward.x;
		listener.OrientFront.y = listenerForward.y;
		listener.OrientFront.z = listenerForward.z;
		listener.OrientTop.x   = listenerUp.x;
		listener.OrientTop.y   = listenerUp.y;
		listener.OrientTop.z   = listenerUp.z;
		listener.Position.x    = position.x;
		listener.Position.y    = position.y;
		listener.Position.z    = position.z;
		listener.Velocity.x    = velocity.x;
		listener.Velocity.y    = velocity.y;
		listener.Velocity.z    = velocity.z;
	}

	void AudioCommand::Calculate3DSoundProperties(X3DAUDIO_EMITTER const& audioEmitter, AudioSourceVoice& audioSourceVoice, X3DAUDIO_DSP_SETTINGS& audioDspSettings) const NABI_NOEXCEPT
	{
		IXAudio2SourceVoice* const sourceVoice = audioSourceVoice.GetSourceVoice();

		X3DAudioCalculate(m_XAudioObjects.m_X3DAudio, &m_XAudioObjects.m_Listener, &audioEmitter,                               // Instances
			X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB, // Flags
			&audioDspSettings																								    // Settings
		);

		DX_ASSERT(sourceVoice->SetOutputMatrix(
			m_XAudioObjects.m_MasteringVoice,    // Destination voice
			GetSourceChannelCount(),             // Source channels
			GetInputChannelCount(),              // Destination channels
			audioDspSettings.pMatrixCoefficients // Level matrix	
		));

		DX_ASSERT(sourceVoice->SetFrequencyRatio(audioDspSettings.DopplerFactor));

		// From https://learn.microsoft.com/en-us/windows/win32/api/xaudio2/ns-xaudio2-xaudio2_filter_parameters
		XAUDIO2_FILTER_PARAMETERS const filterParameters = { 
			LowPassFilter,                          // Filter type
			2.0f *                                  // (part of the equation)
			sinf(X3DAUDIO_PI / 6.0f *               // " "
			audioDspSettings.LPFDirectCoefficient), // Desired filter cutoff frequency
			1.0f                                    // Controls how quickly frequencies beyond the set frequency are dampened
		};
		DX_ASSERT(sourceVoice->SetFilterParameters(&filterParameters));
	}

    HRESULT AudioCommand::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) const NABI_NOEXCEPT
    {
		HRESULT hr = S_OK;
		DX_ASSERT(SetFilePointer(
			hFile,     // File
			0l,        // Distance to move
			NULL,      // Distance to move high
			FILE_BEGIN // Move method
		));
			
		DWORD dwRIFFDataSize = 0ul;
		DWORD bytesRead      = 0ul;
		DWORD dwOffset       = 0ul;
		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwFileType;

		while (hr == S_OK)
		{
			DWORD dwRead;
			DX_ASSERT(ReadFile(
				hFile,		   // File
				&dwChunkType,  // Buffer
				sizeof(DWORD), // Number of bytes to read
				&dwRead,       // Number of bytes read
				NULL           // Overlapped?
			));

			DX_ASSERT(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)); // ""

			switch (dwChunkType)
			{
			case c_FourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4ul;

				DX_ASSERT(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)); //  ""
				break;

			default:
				DX_ASSERT(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)); //  ""
				break;
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) // bytesRead is never set??
			{
				return S_FALSE;
			}
		}

		return S_OK;
    }

	HRESULT AudioCommand::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) const NABI_NOEXCEPT
	{
		DX_ASSERT(SetFilePointer(
			hFile,        // File
			bufferoffset, // Distance to move
			NULL,         // Distance to move high
			FILE_BEGIN    // Move method
		));

		DWORD dwRead;
		DX_ASSERT(ReadFile(
			hFile,      // File
			buffer,     // Buffer
			buffersize, // Number of bytes to read
			&dwRead,    // Number of bytes read
			NULL        // Overlapped?
		));

		return S_OK; // Would have thrown before this point anyways
	}
} // namespace nabi::Audio
