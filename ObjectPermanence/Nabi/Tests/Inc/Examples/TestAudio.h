#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "AssetBank.h"
#include "CoreSingletonComponents\AudioStateComponent.h"
#include "ResourceBank.h"

#ifdef RUN_TESTS

namespace ecs
{
	class AudioSystem;
	class InputSystem;
} // namespace ecs

namespace nabi::Tests::Examples
{
	class TestAudio final : public IExample
	{
	public:
		enum class AudioIDs : ecs::SComp::AudioStateComponent::AudioID
		{
			BingoBangoBongo,
			BingoBangoBongoTwo
		};

		TestAudio(nabi::Context& context);
		~TestAudio();

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		class TestAssetBank final : public nabi::Resource::AssetBank
		{
		public:
			TestAssetBank(nabi::Context& context);
			virtual ~TestAssetBank() override;

			virtual bool LoadAssets() override;
			virtual bool UnloadAssets() override;

		private:
			bool LoadAudioEffects();

			nabi::Resource::ResourceBank<nabi::Audio::AudioEffect, nabi::Audio::AudioEffectLoader, 20u> m_AudioEffectBank;
		};

		void ParseXmlAudioResource();

		nabi::Context& m_Context;
		std::unique_ptr<TestAssetBank> m_AssetBank;

		std::unique_ptr<ecs::AudioSystem> m_AudioSystem;
		std::unique_ptr<ecs::InputSystem> m_InputSystem;

		entt::entity m_AudioEmitterEntity;
	};
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
