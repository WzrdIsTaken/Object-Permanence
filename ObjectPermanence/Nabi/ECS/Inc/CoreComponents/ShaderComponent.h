#pragma once
#include "Core.h"

#include "ComponentBase.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "ResourceWrappers.h"

namespace ecs
{
	struct ShaderComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::PixelShader> m_PixelShaderResource;
		nabi::Resource::ResourceRef<nabi::Rendering::VertexShader> m_VertexShaderResource;

		ShaderComponent()
			: m_PixelShaderResource{}
			, m_VertexShaderResource{}
		{
		}
	};
} // namespace ecs
