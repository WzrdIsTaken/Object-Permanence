#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	struct LightingProperties abstract
	{
		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularAttenuation;
		float m_SpecularIntensity;
	};

	struct DirectionalLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
		dx::XMFLOAT3 m_Direction;
		dx::XMFLOAT3 m_Colour; // float4?
		float m_Intensity;

		DirectionalLightComponent()
			: LightingProperties{1.0f, 1.0f, 1.0f, 1.0f}
			, m_Direction{}
			, m_Colour{}
			, m_Intensity(1.0f)
		{
		}
	};

	struct SpotLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
		// (currently unused)
	};

	struct PointLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
		// (currently unused)
	};
} // namespace ecs
