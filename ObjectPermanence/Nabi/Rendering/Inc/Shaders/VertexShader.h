#pragma once
#include "EngineCore.h"

#include <memory>
#include <string>
#include <vector>

#include "DirectXIncludes.h"

#include "Buffers\ConstantBuffer.h" // TODO - Can we forward declare the enum?

namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::Rendering
{
	namespace Layouts
	{
		inline std::vector<D3D11_INPUT_ELEMENT_DESC> const c_MeshInputLayout // Perhaps this could be reworked into an array?
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		inline std::vector<D3D11_INPUT_ELEMENT_DESC> const c_SpriteInputLayout
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}

	struct VertexShader final
	{
		wrl::ComPtr<ID3D11VertexShader> m_Shader;
		wrl::ComPtr<ID3D11InputLayout> m_Layout;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};

	class VertexShaderLoader final
	{
	public:
		typedef std::shared_ptr<VertexShader> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;

		void SetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputLayout) NABI_NOEXCEPT;
		void SetConstantBuffers(std::vector<ConstantBufferIndex::Enum> const& constantBuffers) NABI_NOEXCEPT;

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputLayout = {};
		std::vector<ConstantBufferIndex::Enum> m_ConstantBuffers = {};
	};
} // namespace nabi::Rendering
