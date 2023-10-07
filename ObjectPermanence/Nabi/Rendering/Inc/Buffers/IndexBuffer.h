#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

// Forward Declares
namespace nabi
{
	struct Context;
} // namespace nabi
namespace nabi::Rendering
{
	struct RenderBufferData;
} // namespace nabi::Rending

namespace nabi::Rendering
{
	struct IndexBuffer final
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_ByteWidth;
	};

	class IndexBufferLoader final
	{
	public:
		[[nodiscard]] IndexBuffer operator()(RenderBufferData const& meshData, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
