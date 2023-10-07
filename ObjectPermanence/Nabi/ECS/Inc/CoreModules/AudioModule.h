#pragma once
#include "Core.h"

#include <optional>

#include "Context.h"
#include "CoreSingletonComponents\AudioStateComponent.h"

namespace ecs
{
	struct AudioEmitterComponent;
} // namespace ecs
namespace nabi::Audio
{
	class AudioSourceVoice;
} // namespace nabi::Audio

namespace ecs::AudioModule
{
	struct PlaySettings final
	{
		std::optional<float> m_Volume;
	};
	PlaySettings constexpr c_DefaultPlaySettings =
	{
		.m_Volume = 1.0f
	};

	// --- Pool ---

	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize2D, size_t const poolSize3D);
	void DestroyAllEffects(nabi::Context& context);
	void DestroyAllVoices(nabi::Context& context);

	[[nodiscard]] SComp::AudioStateComponent::AudioSource* GetFirstReadyAudioSourceFromPool(nabi::Context& context, 
		SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool);

	// --- Loading ---

	void MapLoadedAudioEffectToID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, 
		nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect);

	// --- Playing ---

	SComp::AudioStateComponent::AudioSource* const Play2DAudioEffect(nabi::Context& context, 
		SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings);
	SComp::AudioStateComponent::AudioSource* const Play3DAudioEffect(nabi::Context& context, 
		AudioEmitterComponent& audioEmitterComponent, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings);

	void StopAudioEffect(nabi::Context const& context, SComp::AudioStateComponent::AudioSource& audioSource);
	void StopAll2DAudioEffectsByID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID);
	void StopAll3DAudioEffectsByID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID);

	// --- State ---

	[[nodiscard]] inline SComp::AudioStateComponent const& GetAudioStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::AudioStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Audio));
	}
	[[nodiscard]] inline SComp::AudioStateComponent& GetAudioStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::AudioStateComponent&>(GetAudioStateComponent(const_cast<nabi::Context const&>(context)));
	}
} // namespace ecs::AudioModule
