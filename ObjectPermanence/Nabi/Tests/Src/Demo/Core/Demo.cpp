#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\Core\Demo.h"

#include "Demo\Core\DemoEnums.h"

namespace core
{
	Demo::Demo(HINSTANCE const hInstance, nabi::NabiCoreSettings const& initSettings)
		: nabi::NabiCore(hInstance, initSettings)
		, m_AssetBank(std::make_unique<DemoAssetBank>(m_Context))
	{
	}

	int Demo::Init()
	{
		int result = nabi::NabiCore::Init();
		result &= static_cast<int>(m_AssetBank->LoadAssets(core::AssetType::All));

		return result;
	}

	void Demo::RefreshLoadedAssets(core::AssetType const assetType) const
	{
		/*
		* When entities are created from templates or as part of an entity group, then they will have 
		* the resource component - not the components with asset pointers. So we need to perform the
		* load asset code again. This will be fast, as the underlying resource will have already been
		* loaded. We are just adding/removing a couple components / pointing pointers at memory. 
		*/

		ASSERT(m_AssetBank->RefreshLoadedAssets(assetType), "Failed to refresh loaded assets");
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
