#include "GameCore.h"

// cpp's Header 
#include "Game.h"

namespace core
{
	Game::Game(HINSTANCE const hInstance, nabi::NabiCoreSettings const& initSettings)
		: nabi::NabiCore(hInstance, initSettings)
		, m_AssetBank(std::make_unique<GameAssetBank>(m_Context))
	{
	}

	s32 Game::Init()
	{
		s32 result = nabi::NabiCore::Init();
		result &= static_cast<s32>(m_AssetBank->LoadAssets(core::AssetType::All));

		return result;
	}

	void Game::RefreshLoadedAssets(core::AssetType const assetType) const
	{
		// When entities are created from templates or as part of an entity group, then they will have
		// the resource component - not the components with asset pointers. So we need to perform the
		// load asset code again. This will be fast, as the underlying resource will have already been
		// loaded. We are just adding/removing a couple components / pointing pointers at memory.

		ASSERT(m_AssetBank->RefreshLoadedAssets(assetType), "Failed to refresh loaded assets");
	}
} // namespace core
