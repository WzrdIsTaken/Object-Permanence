#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "DirectXIncludes.h"

namespace nabi::Rendering
{
	/// <summary>
	/// A collection of all the DirectX stuff needed for rendering
	///
	/// Why is m_RenderTargetView a raw pointer? Because for some reason it falls out of scope (somewhere in RenderCommand I guess..) but I don't see why!!
	/// Making it a raw pointer is a quick and dirty fix. It doesn't really matter.. it doesn't get deleted until shutdown anyway... but still - not hot
	/// </summary>
	struct DXObjects final
	{
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		/*wrl::ComPtr<ID3D11RenderTargetView>*/ ID3D11RenderTargetView* m_RenderTargetView;
		wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

		D3D11_VIEWPORT m_Viewport;

#ifdef USE_DEBUG_UTILS
		std::shared_ptr<ID3D11Debug> m_DebugDevice;
#endif // ifdef USE_DEBUG_UTILS
	};

	DXObjects const c_DXObjectsDefaultSettings
	{
		.m_Device = nullptr,
		.m_SwapChain = nullptr,
		.m_Context = nullptr,
		.m_RenderTargetView = nullptr,
		.m_DepthStencilView = nullptr,

		.m_Viewport{}

#ifdef USE_DEBUG_UTILS
		, .m_DebugDevice = nullptr
#endif // ifdef USE_DEBUG_UTILS
	};
} // namespace nabi::Rendering
