#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"
#include "DirectXIncludes.h"

#include "Buffers/ConstantBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Containers/Colour.h"
#include "Containers/Vertex.h"
#include "Shaders/PixelShader.h"
#include "Shaders/VertexShader.h"
#include "Textures/Sampler.h"
#include "Textures/Texture.h"
#include "TypeUtils.h"

// Forward Declares
namespace nabi
{
	struct WindowSettings;
} // namespace nabi
namespace nabi::Rendering
{
	struct ConstantBuffer;
	struct DXObjects;
} // namespace nabi::Rendering

namespace nabi::Rendering
{
	/// <summary>
	/// A basic cache so we don't bind unnecessary
	/// Its usage can be toggled on/off with USE_RENDER_CACHE (defined in the cpp)
	/// </summary>
	struct RenderCommandCache final
	{
		ID3D11VertexShader* m_LastBoundVertexShader;
		ID3D11PixelShader* m_LastBoundPixelShader;

		ID3D11Buffer* m_LastBoundVertexBuffer;
		ID3D11Buffer* m_LastBoundIndexBuffer;

		ID3D11ShaderResourceView* m_LastBoundTexture;
		ID3D11SamplerState* m_LastBoundSampler;

		RenderCommandCache() NABI_NOEXCEPT
			: m_LastBoundVertexShader(nullptr)
			, m_LastBoundPixelShader(nullptr)
			, m_LastBoundVertexBuffer(nullptr)
			, m_LastBoundIndexBuffer(nullptr)
			, m_LastBoundTexture(nullptr)
			, m_LastBoundSampler(nullptr)
		{
		}
	};

	/// <summary>
	/// The RenderCommand talks to DirectX to do all things rendering.
	/// Apparently a RenderCommand is actually something different and this should be called like 'RenderCommandCentre' - but 'RenderCommand' sounds cooler :D
	/// </summary>
	class RenderCommand final
	{
	public:
		// --- Initialization ---

		RenderCommand(DXObjects& dxObjects, HWND const hWnd, WindowSettings const& settings) NABI_NOEXCEPT;
		~RenderCommand() NABI_NOEXCEPT;

		// --- Frame ---

		void BeginFrame() const NABI_NOEXCEPT;
		void EndFrame() const NABI_NOEXCEPT;

		// --- Back Buffer ---

		void Clear() const NABI_NOEXCEPT;
		void Swap() const NABI_NOEXCEPT;
		void SetBackgroundColour(Colour const& colour) NABI_NOEXCEPT;

		// --- Creation ---

		[[nodiscard]] ConstantBuffer CreateConstantBuffer(UINT const byteWidth) const NABI_NOEXCEPT;
		[[nodiscard]] IndexBuffer CreateIndexBuffer(std::vector<UINT> const& triangles) const NABI_NOEXCEPT;
		[[nodiscard]] VertexBuffer CreateVertexBuffer(std::vector<Vertex> const& vertices) const NABI_NOEXCEPT;

		[[nodiscard]] PixelShader CreatePixelShader(std::string const& filePath) const NABI_NOEXCEPT;
		[[nodiscard]] VertexShader CreateVertexShader(std::string const& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC> const& layout) const NABI_NOEXCEPT;

		[[nodiscard]] Texture CreateTexture(std::string const& filePath) const NABI_NOEXCEPT;
		[[nodiscard]] Sampler CreateSampler() const NABI_NOEXCEPT;

		[[nodiscard]] wrl::ComPtr<ID3D11Buffer> CreateStagingResource(UINT const* const byteWidth, UINT const* const structureByteStride) const NABI_NOEXCEPT;

		// --- Binding ---

		void BindPixelShader(PixelShader const& pixelShader) NABI_NOEXCEPT;
		void BindVertexShader(VertexShader const& vertexShader) NABI_NOEXCEPT;

		void BindIndexBuffer(IndexBuffer const& indexBuffer) NABI_NOEXCEPT;
		void BindVertexBuffer(VertexBuffer const& vertexBuffer) NABI_NOEXCEPT;

		void BindTexture(Texture const& texture) NABI_NOEXCEPT;
		void BindSampler(Sampler const& sampler) NABI_NOEXCEPT;

		// --- Updating ---

		void UpdateConstantBuffer(ConstantBuffer const& constantBuffer, void const* const data) const NABI_NOEXCEPT;

		void UpdateBuffer(wrl::ComPtr<ID3D11Buffer> const buffer, UINT const bufferDataSize, std::function<void(D3D11_MAPPED_SUBRESOURCE&)> const& action) const NABI_NOEXCEPT;
		void UpdateBuffer(wrl::ComPtr<ID3D11Buffer> const buffer, D3D11_MAP const mapType, std::function<void(D3D11_MAPPED_SUBRESOURCE&)> const& action) const NABI_NOEXCEPT;

		void CopyBufferResource(wrl::ComPtr<ID3D11Buffer> const destination, wrl::ComPtr<ID3D11Buffer> const source) const NABI_NOEXCEPT;

		bool UpdateWindowSize(UINT const newWidth, UINT const newHeight) const NABI_NOEXCEPT;

		// --- Access ---

		dx::XMFLOAT2 GetTextureDimensions(Texture const& texture) const NABI_NOEXCEPT;

		// --- Drawing ---

		void Draw(UINT const indexCount) const NABI_NOEXCEPT;
		void DrawIndexed(UINT const indexCount) const NABI_NOEXCEPT;

		[[nodiscard]] UINT ExtractTriangleCountFromIndexBuffer(IndexBuffer const& indexBuffer) const NABI_NOEXCEPT;

		// --- Getters ---

		[[nodiscard]] RenderCommandCache const& GetCache() const NABI_NOEXCEPT;
		[[nodiscard]] Colour GetBackgroundColour() const NABI_NOEXCEPT;

		// --- Setters ---

		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY const primitiveTopology) const NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(RenderCommand)

		[[nodiscard]] D3D11_MAPPED_SUBRESOURCE MapBuffer(wrl::ComPtr<ID3D11Buffer> const buffer, D3D11_MAP const mapType) const NABI_NOEXCEPT;
		void UnmapBuffer(wrl::ComPtr<ID3D11Buffer> const buffer) const NABI_NOEXCEPT;

		DXObjects& m_DXObjects;
		RenderCommandCache m_Cache;
		std::array<FLOAT, 4> m_BackgroundColour;
	};
} // namespace nabi::Rendering
