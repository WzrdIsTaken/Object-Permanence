#pragma once
#include "GameCore.h"

// Game Headers
#include "Typedefs\IntegerTypes.h"

/**
 * Asset categories, used in GameAssetBank.
*/

namespace core
{
	enum class AssetType : u64
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
		auto constexpr c_AudioAssets = AssetType::Audio;

		inline constexpr bool ValidAssetType(AssetType const assetTypeOne, AssetType const assetTypeTwo) noexcept;
	} // namespace AssetTypes
} // namespace core

// Include Inline
#include "Core\Inl\Identifiers\AssetTypes.inl"
