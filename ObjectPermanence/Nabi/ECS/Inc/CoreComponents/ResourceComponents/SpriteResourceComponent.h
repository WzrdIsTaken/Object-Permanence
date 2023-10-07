#pragma once
#include "Core.h"

#include "ComponentBase.h"
#include "Containers\UVs.h"

namespace ecs::RComp
{
	struct SpriteResourceComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_ImagePath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		// For sprite sheets 
		nabi::Rendering::UVs m_UVs;

		SpriteResourceComponent()
			: m_ImagePath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_UVs(nabi::Rendering::UV::c_DefaultSpriteUVs)
		{
		}
	};
} // namespace ecs::RComp
