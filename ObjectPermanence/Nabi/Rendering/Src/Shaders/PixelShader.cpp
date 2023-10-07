#include "EngineCore.h"

#include "WinIncludes.h"

#include "Shaders\PixelShader.h"

#include "Context.h"

namespace nabi::Rendering
{
	PixelShaderLoader::ResourceType PixelShaderLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		PixelShader pixelShader = context.m_RenderCommand->CreatePixelShader(resourcePath);
		ConstantBufferLoader::AssignConstantBuffersToShader(pixelShader.m_ConstantBuffers, m_ConstantBuffers, ConstantBufferLoader::AddMode::ClearAndAdd, context);

		return std::make_shared<PixelShader>(pixelShader);
	}

	void PixelShaderLoader::SetConstantBuffers(std::vector<ConstantBufferIndex::Enum> const& constantBuffers) NABI_NOEXCEPT
	{
		m_ConstantBuffers = constantBuffers;
	}
} // namespace nabi::Rendering
