#pragma once
#include "Core.h"

#include "ComponentBase.h"

namespace ecs::RComp
{
	struct ModelResourceComponent final : public nabi::ECS::ComponentBase
	{
		std::string m_MeshPath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;
		std::string m_TexturePath;

		ModelResourceComponent()
			: m_MeshPath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_TexturePath("InvalidPath")
		{
		}
	};
} // namespace ecs::RComp
