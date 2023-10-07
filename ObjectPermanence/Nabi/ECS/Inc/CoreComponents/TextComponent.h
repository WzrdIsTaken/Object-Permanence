#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	// This component gets created from the data in a TextResourceComponent

	struct TextComponent final : public nabi::ECS::ComponentBase
	{
		entt::hashed_string m_Content;
		std::vector<entt::entity> m_Characters;
		dx::XMFLOAT2 m_CharacterSpace;

		int m_AsciiShift;
		dx::XMINT2 m_TextureAtlas;

		int m_CharacterPoolSize;
		int m_ActiveInPool;

		TextComponent()
			: m_Content("InvalidContent"_hs)
			, m_Characters{}
			, m_CharacterSpace{0.0f, 0.0f}
			, m_AsciiShift(0)
			, m_TextureAtlas{0, 0}
			, m_CharacterPoolSize(0)
			, m_ActiveInPool(0)
		{
		}
	};
} // namespace ecs
