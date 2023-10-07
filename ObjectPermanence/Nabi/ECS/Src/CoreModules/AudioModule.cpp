#include "Core.h"

#include "CoreModules\AudioModule.h"

#include "CoreComponents\AudioEmitterComponent.h"

namespace ecs::AudioModule
{
	namespace
	{
		SComp::AudioStateComponent::AudioSource* const PlayAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID,
			PlaySettings const& playSettings, SComp::AudioStateComponent::SourceVoicePool& voicePool, 
			std::optional<std::function<void(nabi::Audio::AudioSourceVoice&)>> configureSourceVoice)
		{
			SComp::AudioStateComponent::AudioSource* selectedAudioSource = nullptr;
			SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

			auto effect = audioEffects.find(audioID);
			if (effect != audioEffects.end())
			{
				SComp::AudioStateComponent::AudioSource* const audioSource = GetFirstReadyAudioSourceFromPool(context, voicePool);
				if (audioSource)
				{
					nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect = effect->second;
					if (audioEffect.IsValid())
					{
						audioSource->m_ID = audioID;
						context.m_AudioCommand->LoadAudioVoice(*audioEffect.GetResourceNonConst(), audioSource->m_Voice);

						nabi::Audio::AudioSourceVoice& audioSourceVoice = audioSource->m_Voice;
						IXAudio2SourceVoice* const sourceVoice = audioSourceVoice.GetSourceVoice();
						if (configureSourceVoice)
						{
							(*configureSourceVoice)(audioSourceVoice);
						}

						if (playSettings.m_Volume.has_value())
						{
							sourceVoice->SetVolume(playSettings.m_Volume.value());
						}
						// ...any other settings

						selectedAudioSource = audioSource;
						sourceVoice->Start();
					}
					else
					{
						ASSERT_FAIL("Trying to play an audio effect with ID " << WRAP(audioID, "'") << "but its null! Did the resource get removed a resource bank?");
					}
				}
			}
			else
			{
				ASSERT_FAIL("Trying to play an audio effect with ID " << WRAP(audioID, "'") << " but no effect with that id is loaded");
			}

			return selectedAudioSource;
		}

		void StopAllAudioEffectsByID(nabi::Context const& context, SComp::AudioStateComponent::AudioID const audioID,
			SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool)
		{
			for (auto& audioSource : sourceVoicePool)
			{
				if (audioSource.m_ID == audioID)
				{
					StopAudioEffect(context, audioSource);
				}
			}
		}
	}

	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize2D, size_t const poolSize3D)
	{
		ASSERT_CODE
		(
			static bool poolInitialized = false;

			ASSERT(!poolInitialized, "Initializing the audio source voice pool more than once");
			poolInitialized = true;
		)

		GetAudioStateComponent(context).m_2DSourceVoicePool.resize(poolSize2D);
		GetAudioStateComponent(context).m_3DSourceVoicePool.resize(poolSize3D);
	}

	void DestroyAllEffects(nabi::Context& context)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

		for (auto& [audioID, audioEffect] : audioEffects)
		{
			if (audioEffect.IsValid())
			{
				context.m_AudioCommand->DestroyAudioEffect(*audioEffect.GetResourceNonConst());
			}
			else
			{
				LOG(LOG_PREP, LOG_ERROR, LOG_CATEGORY_AUDIO, 
					"Tried to destroy an audio effect, but the audio effect was null. This may result in a bad shutdown when xaudio terminates", LOG_END);
			}
		}
	}

	void DestroyAllVoices(nabi::Context& context)
	{
		auto destroyAllVoicesHelper =
			[&context](SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool) -> void
			{
				for (auto& sourceVoice : sourceVoicePool)
				{
					sourceVoice.m_ID = SComp::AudioStateComponent::c_NullAudioID;
					context.m_AudioCommand->DestroyAudioVoice(sourceVoice.m_Voice);
				}
			};

		destroyAllVoicesHelper(GetAudioStateComponent(context).m_2DSourceVoicePool);
		destroyAllVoicesHelper(GetAudioStateComponent(context).m_3DSourceVoicePool);
	}

	SComp::AudioStateComponent::AudioSource* GetFirstReadyAudioSourceFromPool(nabi::Context& /*context*/, SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool)
	{
		SComp::AudioStateComponent::AudioSource* audioSource = nullptr;

		ASSERT(!sourceVoicePool.empty(), "Trying to get a source voice from the pool, but the pool is empty");

		for (auto& source : sourceVoicePool)
		{
			if (source.m_Voice.IsReady())
			{
				source.m_ID = SComp::AudioStateComponent::c_NullAudioID;
				audioSource = &source;
				break;
			}
		}

		ASSERT(audioSource, "No available audio source voices! The audio clip may not play on time, or not at all");
		// To fix this ^, just increase AudioStateComponent::m_SourceVoicePool's size (via the call to InitSourceVoicePool)

		return audioSource;
	}

	// 

	void MapLoadedAudioEffectToID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;
		if (!audioEffects.contains(audioID))
		{
			audioEffects.emplace(audioID, audioEffect);
		}
	}

	// 

	SComp::AudioStateComponent::AudioSource* const Play2DAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings)
	{
		return PlayAudioEffect(context, audioID, playSettings, GetAudioStateComponent(context).m_2DSourceVoicePool, std::nullopt /*no custom play behaviour required*/);
	}

	SComp::AudioStateComponent::AudioSource* const Play3DAudioEffect(nabi::Context& context, AudioEmitterComponent& audioEmitterComponent,
		SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings)
	{
		return PlayAudioEffect(context, audioID, playSettings, GetAudioStateComponent(context).m_3DSourceVoicePool,
			[&context, &audioEmitterComponent](nabi::Audio::AudioSourceVoice& audioSourceVoice) -> void
			{
				context.m_AudioCommand->Calculate3DSoundProperties(
					audioEmitterComponent.m_Emitter,
					audioSourceVoice,
					audioEmitterComponent.m_DSPSettings
				);
			});
	}

	void StopAudioEffect(nabi::Context const& /*context*/, SComp::AudioStateComponent::AudioSource& audioSource)
	{
		IXAudio2SourceVoice* const sourceVoice = audioSource.m_Voice.GetSourceVoice();

		if (sourceVoice)
		{
			audioSource.m_ID = SComp::AudioStateComponent::c_NullAudioID;
			DX_ASSERT(sourceVoice->Stop());
			DX_ASSERT(sourceVoice->FlushSourceBuffers());
		}
		else
		{
			ASSERT_FAIL("Trying to stop an audio effect, but its IXAudio2SourceVoice is null");
		}
	}

	void StopAll2DAudioEffectsByID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID)
	{
		StopAllAudioEffectsByID(context, audioID, GetAudioStateComponent(context).m_2DSourceVoicePool);
	}

	void StopAll3DAudioEffectsByID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID)
	{
		StopAllAudioEffectsByID(context, audioID, GetAudioStateComponent(context).m_3DSourceVoicePool);
	}
} // namespace ecs::AudioModule

/*
* This logic is now handled by a special ResourceLoader, it is deprecated
* 
	void LoadAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, std::string const& path,
		nabi::Audio::AudioCommand::LoadSettings const& loadSettings)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;
		bool const effectAlreadyLoaded = audioEffects.find(audioID) != audioEffects.end();

		if (!effectAlreadyLoaded)
		{
			nabi::Audio::AudioEffect audioEffect;
			context.m_AudioCommand->LoadAudioEffect(audioEffect, path, loadSettings);

			audioEffects.emplace(audioID, std::move(audioEffect));
		}
		else
		{
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_AUDIO << "Loading an audio effect with ID " << WRAP(audioID, "'") << " but its already loaded" << ENDLINE);
		}
	}
*/
