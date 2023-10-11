#include "GameCore.h"

// inl's Header
#include "Identifiers\AssetTypes.h"

namespace core::AssetTypes
{
	inline constexpr bool ValidAssetType(AssetType const assetTypeOne, AssetType const assetTypeTwo) noexcept
	{
		return static_cast<bool>(assetTypeOne & assetTypeTwo);
	}
} // namespace core::AssetTypes
