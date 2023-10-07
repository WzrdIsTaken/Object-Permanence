#include "EngineCore.h"

#include "WinIncludes.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\ConstantBuffer.h"

#include "Context.h"
#include "CoreSingletonComponents\GraphicsComponent.h"

namespace nabi::Rendering
{
	ConstantBuffer nabi::Rendering::ConstantBufferLoader::operator()(UINT const byteWidth, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ConstantBuffer const constantBuffer = context.m_RenderCommand->CreateConstantBuffer(byteWidth);
		return constantBuffer;
	}

	void ConstantBufferLoader::AssignConstantBuffersToShader(
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>>& shaderConstantBuffers, std::vector<ConstantBufferIndex::Enum> const& constantBuffersToAssign, 
		AddMode const addMode, nabi::Context const& context) NABI_NOEXCEPT
	{
		entt::entity const graphicEntity = context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);
		ecs::SComp::GraphicsComponent const& graphicsComponent = context.m_Registry.get<ecs::SComp::GraphicsComponent>(graphicEntity);
		std::array<nabi::Rendering::ConstantBuffer, nabi::Rendering::ConstantBufferIndex::ENUM_COUNT> const& constantBuffers = graphicsComponent.m_ConstantBuffers;

		if (addMode == AddMode::ClearAndAdd)
		{
			shaderConstantBuffers.clear();
			shaderConstantBuffers.reserve(constantBuffersToAssign.size());
		}

		for (ConstantBufferIndex::Enum const constantBuffer : constantBuffersToAssign)
		{
			wrl::ComPtr<ID3D11Buffer> const& constantBufferPtr = constantBuffers.at(constantBuffer).m_Buffer;
			bool const bufferAlreadyAdded = std::find(shaderConstantBuffers.cbegin(), shaderConstantBuffers.cend(), constantBufferPtr) != shaderConstantBuffers.cend();

			if (!bufferAlreadyAdded)
			{
				shaderConstantBuffers.push_back
				(
					constantBuffers.at(constantBuffer).m_Buffer
				);
			}
			else
			{
				LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_RENDERING, "Trying to add constant buffer of type " << constantBuffer << " to a shader when it already exists!", LOG_END);
			}
		}
	}
} // namespace nabi::Rendering
