#pragma once
#include "Core.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"
#include "ComponentBase.h"
#include "ResourceWrappers.h"

namespace ecs::SComp
{
	struct AudioStateComponent final : public nabi::ECS::ComponentBase
	{
		typedef uint32_t AudioID; // The enum which holds sounds should be of this type, and reflected with REFLECT_ENUM_UNDERLYING
		static AudioID constexpr c_NullAudioID = std::numeric_limits<AudioID>::max();

		struct AudioSource final
		{
			AudioID m_ID;
			nabi::Audio::AudioSourceVoice m_Voice;

			AudioSource()
				: m_ID(c_NullAudioID)
				, m_Voice{}
			{
			}
		};

		typedef std::unordered_map<AudioID, nabi::Resource::ResourceRef<nabi::Audio::AudioEffect>> AudioEffectLookup;
		typedef std::vector<AudioSource> SourceVoicePool;

		AudioEffectLookup m_AudioEffects;
		SourceVoicePool m_2DSourceVoicePool;
		SourceVoicePool m_3DSourceVoicePool;

		AudioStateComponent()
			: m_AudioEffects{}
			, m_2DSourceVoicePool{}
			, m_3DSourceVoicePool{}
		{
		}
	};
} // namespace ecs::SComp
