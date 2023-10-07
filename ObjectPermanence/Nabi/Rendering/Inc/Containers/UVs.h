#pragma once
#include "EngineCore.h"

#include <string>
#include <vector>

#include "Buffers\ConstantBuffer.h"

namespace nabi::Rendering
{
	struct UVs
	{
		float m_U1;
		float m_U2;
		float m_V1;
		float m_V2;

		static UVs FromString(std::string const& string) NABI_NOEXCEPT;
	};

	namespace UV
	{
		UVs constexpr c_DefaultSpriteUVs = UVs{ 1.0f, 1.0f, 0.0f, 0.0f };

		// All sprite uvs in nabi follow the same pattern so can use this helper function
		[[nodiscard]] std::vector<dx::XMFLOAT2> constexpr CreateSpriteUVs(UVs const& uvs) NABI_NOEXCEPT
		{
			return std::vector<dx::XMFLOAT2>
			{
				// Bottom left
				{ uvs.m_V1, uvs.m_U2 },
				// Top left
				{ uvs.m_V1, uvs.m_V2 },
				// Top right
				{ uvs.m_U1, uvs.m_V2 },
				// Bottom right
				{ uvs.m_U1, uvs.m_U2 }
			}; 
		}
	}; // namespace UVs
} // namespace nabi::Rendering
