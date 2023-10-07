#include "EngineCore.h"

#include "WinIncludes.h"

#include "Shaders\VertexShader.h"

#include "Context.h"

namespace nabi::Rendering
{
	VertexShaderLoader::ResourceType VertexShaderLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		VertexShader vertexShader = context.m_RenderCommand->CreateVertexShader(resourcePath, m_InputLayout);
		ConstantBufferLoader::AssignConstantBuffersToShader(vertexShader.m_ConstantBuffers, m_ConstantBuffers, ConstantBufferLoader::AddMode::ClearAndAdd, context);

		return std::make_shared<VertexShader>(vertexShader);
	}

	void VertexShaderLoader::SetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputLayout) NABI_NOEXCEPT
	{
		m_InputLayout = inputLayout;
	}

	void VertexShaderLoader::SetConstantBuffers(std::vector<ConstantBufferIndex::Enum> const& constantBuffers) NABI_NOEXCEPT
	{
		m_ConstantBuffers = constantBuffers;
	}
} // namespace nabi::Rendering
