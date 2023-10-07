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
	struct PixelShader final
	{
		wrl::ComPtr<ID3D11PixelShader> m_Shader;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};

	class PixelShaderLoader final
	{
	public:
		typedef std::shared_ptr<PixelShader> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context)  const NABI_NOEXCEPT;

		void SetConstantBuffers(std::vector<ConstantBufferIndex::Enum> const& constantBuffers) NABI_NOEXCEPT;

	private:
		std::vector<ConstantBufferIndex::Enum> m_ConstantBuffers = {};
	};
} // namespace nabi::Rendering
