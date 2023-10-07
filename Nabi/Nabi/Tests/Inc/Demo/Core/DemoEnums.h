#pragma once
#include "Demo\DemoCore.h"

// Should split these out into seperate files in the real thing

#ifdef INCLUDE_DEMO

#include "CoreComponents\ColliderComponent.h"
#include "CoreSingletonComponents\AudioStateComponent.h"

namespace core
{	 
	enum class AssetType : uint64_t
	{
		Model  = 1 << 1,
		Sprite = 1 << 2,
		Text   = 1 << 3,
		Audio  = 1 << 4,

		All = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(AssetType)

	namespace AssetTypes
	{
		auto constexpr c_RenderableAssets = AssetType::Model | AssetType::Sprite | AssetType::Text;
		auto constexpr c_AudioAssets      = AssetType::Audio;
	} // namespace AssetTypes

	// ---

	// Note: Matching masks will collide

	enum class CollisionMask : ecs::ColliderComponent::ColliderMask
	{
		Object = 1 << 1,
		Player = 1 << 1,
		All = ~0,
		ENUM_COUNT
	};
	DEFINE_ENUM_FLAG_OPERATORS(CollisionMask)

	// ---

	enum class AudioID : ecs::SComp::AudioStateComponent::AudioID
	{
		BingoBangoBongo,
		ENUM_COUNT
	};
	DEFINE_ENUM_FLAG_OPERATORS(AudioID)
} // namespace core

#endif //ifdef INCLUDE_DEMO
