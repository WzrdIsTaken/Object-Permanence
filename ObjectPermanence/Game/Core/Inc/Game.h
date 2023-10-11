#pragma once
#include "GameCore.h"

// Nabi Headers
#include "NabiCore.h"
#include "TypeUtils.h"

// Game Headers
#include "GameAssetBank.h" // See https://stackoverflow.com/a/6089065/8890269 for why we can't forward declare this
#include "Typedefs\IntegerTypes.h"

/**
 * The game's entry point, created in main.
*/

namespace nabi
{
	struct NabiCoreSettings;
} // namespace nabi

namespace core
{
	class Game final : public nabi::NabiCore
	{
	public:
		Game(HINSTANCE const hInstance, nabi::NabiCoreSettings const& initSettings);
		~Game() override = default;

		[[nodiscard]] s32 Init() override;

		void RefreshLoadedAssets(core::AssetType const assetType) const;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(Game)

		std::unique_ptr<GameAssetBank> m_AssetBank;
	};
} // namespace core
