#include "TestCore.h"

#include "Examples\TestAudio.h"

#include "pugixml.hpp"

#include "AudioSourceVoice.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreComponents\ResourceComponents\AudioResourceComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\InputModule.h"
#include "CoreSystems\AudioSystem.h"
#include "CoreSystems\InputSystem.h"
#include "WorldConstants.h"
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabi::Tests::Examples
{
	// --- Test Audio ---

	namespace 
	{
		typedef TestAudio::AudioIDs AudioIDs;
		REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(AudioIDs)
			REFLECT_ENUM_VALUE(AudioIDs::BingoBangoBongo, "BingoBangoBongo")
			REFLECT_ENUM_VALUE(AudioIDs::BingoBangoBongoTwo, "BingoBangoBongoTwo")
		REFLECT_ENUM_END(AudioIDs)
	}

	auto constexpr c_AudioID = static_cast<ecs::SComp::AudioStateComponent::AudioID>(TestAudio::AudioIDs::BingoBangoBongo);
	auto constexpr c_AnotherAudioID = static_cast<ecs::SComp::AudioStateComponent::AudioID>(TestAudio::AudioIDs::BingoBangoBongoTwo);

	TestAudio::TestAudio(nabi::Context& context)
		: m_Context(context)
		, m_AssetBank(std::make_unique<TestAssetBank>(context))
		, m_AudioEmitterEntity(entt::null)
	{
	}

	TestAudio::~TestAudio()
	{
		// These cause exit code 3 when called here, but not because of anything audio related. I think the registry is already 
		// destroyed by the time we get to this point, so trying to get the AudioStateComponent doesn't work. 
		// In the actual game code, find an appropriate place for these methods!
		//ecs::AudioModule::DestroyAllEffects(m_Context);
		//ecs::AudioModule::DestroyAllVoices(m_Context);
		// UPDATE 13/06/23 - I put these in ~NabiCore. The audio voice pool is also now created there as well
	}

	bool TestAudio::Init()
	{
		// --- Systems ---

		m_AudioSystem = std::make_unique<ecs::AudioSystem>(m_Context, "Audio"_hs, "NabiAudioTestSystems"_hs);
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiAudioTestSystems"_hs);

		// --- Load Audio ---

		//ecs::AudioResourceComponent audioResource = {};
		//audioResource.m_Resources.emplace(c_AudioID, "Nabi/Tests/Data/Audio/bingo_bango_bongo_bish_bash_bosh.wav");
		//m_Context.m_Registry.emplace<ecs::AudioResourceComponent>(m_Context.m_Registry.create(), audioResource);

		ParseXmlAudioResource();
		m_AssetBank->LoadAssets();

		// --- Create an audio emitter component ---

		m_AudioEmitterEntity = m_Context.m_EntityCreator->CreateEntity();
		m_Context.m_Registry.emplace<ecs::AudioEmitterComponent>(m_AudioEmitterEntity);
		m_Context.m_Registry.emplace<ecs::TransformComponent>(m_AudioEmitterEntity);

		// --- Move the listener ---
		dx::XMFLOAT3 constexpr position = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr rotation = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr velocity = { 0.0f, 0.0f, 0.0f };
		m_Context.m_AudioCommand->SetListener(position, rotation, velocity);

		return true;
	}

	bool TestAudio::Update()
	{
		using namespace nabi::Input;
		InputState const wKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_W);
		InputState const eKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_E);
		InputState const rKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_R);
		InputState const tKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_T);

		if (wKeyState == InputState::Pressed)
		{
			ecs::AudioModule::Play2DAudioEffect(m_Context, c_AudioID, ecs::AudioModule::c_DefaultPlaySettings);
		}
		if (eKeyState == InputState::Pressed)
		{
			auto& audioEmitterComponent = m_Context.m_Registry.get<ecs::AudioEmitterComponent>(m_AudioEmitterEntity);
			ecs::AudioModule::Play3DAudioEffect(m_Context, audioEmitterComponent, c_AudioID, ecs::AudioModule::c_DefaultPlaySettings);
		}
		if (rKeyState == InputState::Pressed)
		{
			ecs::SComp::AudioStateComponent::AudioSource* const source =
				ecs::AudioModule::Play2DAudioEffect(m_Context, c_AnotherAudioID, ecs::AudioModule::c_DefaultPlaySettings);

			if (source)
			{
				ecs::AudioModule::StopAudioEffect(m_Context, *source);
			}
		}
		if (tKeyState == InputState::Pressed)
		{
			ecs::AudioModule::StopAll2DAudioEffectsByID(m_Context, c_AudioID);
			ecs::AudioModule::StopAll3DAudioEffectsByID(m_Context, c_AudioID);
		}

		return true;
	}

	bool TestAudio::Render()
	{
		return false;
	}

	void TestAudio::ParseXmlAudioResource()
	{
		nabi::Reflection::XmlParser xmlParser{};
		std::string const docPath = "Nabi/Tests/Data/Audio/test_audio_entities.xml";

		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, m_Context.m_Registry);
	}

	// --- Asset Bank ---

	TestAudio::TestAssetBank::TestAssetBank(nabi::Context& context)
		: AssetBank(context)
		, m_AudioEffectBank(context)
	{
		
	}

	TestAudio::TestAssetBank::~TestAssetBank()
	{
		UnloadAssets();
	}

	bool TestAudio::TestAssetBank::LoadAssets()
	{
		bool result = true;
		result &= LoadAudioEffects();

		return result;
	}

	bool TestAudio::TestAssetBank::UnloadAssets()
	{
		m_AudioEffectBank.Clear();

		return true;
	}

	bool TestAudio::TestAssetBank::LoadAudioEffects()
	{
		m_Context.m_Registry.view<ecs::RComp::AudioResourceComponent /*const?*/>()
			.each([&](entt::entity const entity, auto& audioResourceComponent) -> void
				{
					for (auto const [audioID, audioPath] : audioResourceComponent.m_Resources.Get())
					{
						nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> audioResource = m_AudioEffectBank.LoadResource(audioPath);
						ecs::AudioModule::MapLoadedAudioEffectToID(m_Context, audioID, audioResource);
					}
				});

		return true;
	}
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS

/*
* I hope I can make the approach I am currently using work...
* Update: We can! But the solution is kinda jank as well xD If only I could reflect containers properly... Should of put more time into this!!!
* 
		ecs::AudioResourceComponent::ResourceContainer const& audioResources = audioResourceComponent.m_AudioResources;
		for (ecs::AudioResourceComponent::ResourceContainer::ConstituentContainer const& entry : audioResources.m_Entries)
		{
			auto const audioID = nabi::Reflection::EnumConverter::StringToEnumUnderlyingValue<TestAudio::AudioIDs>(entry.at(0u));
			std::string const& audioPath = entry.at(1u);

			nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> resource = m_AudioEffectBank.LoadResource(audioPath);
			ecs::AudioModule::MapLoadedAudioEffectToID(m_Context, audioID, resource);
		}
*/
