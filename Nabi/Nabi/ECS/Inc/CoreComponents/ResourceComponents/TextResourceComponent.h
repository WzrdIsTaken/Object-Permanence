#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs::RComp
{
	struct TextResourceComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_FontPath; // This should be a texture atlas of the font to use
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		std::string m_Content; // What the text will display
		dx::XMFLOAT2 m_CharacterSpace; // The space between characters
		int m_AsciiShift; // char - m_AsciiShift = position of the character in the texture atlas
		dx::XMINT2 m_TextureAtlas; // The size of the texture atlas defined in m_FontPath

		int m_CharacterPoolSize; // The max number of characters in the text

		TextResourceComponent()
			: m_FontPath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_Content("")
			, m_CharacterSpace(0.0f, 0.0f)
			, m_AsciiShift(0)
			, m_TextureAtlas(0, 0)
			, m_CharacterPoolSize(16)
		{
		}
	};
} // namespace ecs::RComp
