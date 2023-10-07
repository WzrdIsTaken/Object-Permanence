#pragma once
#include "Core.h"

#include "entt.h"

#include "ComponentBase.h"

namespace ecs
{
	// 1 entity per scene has this component
	struct UISceneComponent final : public nabi::ECS::ComponentBase
	{
		//typedef std::vector<entt::hashed_string> UISceneEntityNames; (i realised this could just be a string - will save me a lot of headaches)
		typedef std::vector<entt::entity> UISceneEntities;

		// Authoring
		entt::hashed_string m_SceneName;
		entt::hashed_string m_UISceneEntityNames;
		bool m_Enabled;

		// Internal
		UISceneEntities m_UISceneEntities;

		UISceneComponent()
			: m_SceneName("InvalidScene"_hs)
			, m_UISceneEntityNames("InvalidNames"_hs)
			, m_UISceneEntities({})
			, m_Enabled(true)
		{
		}

		constexpr bool operator == (UISceneComponent const& other) const
		{
			return m_SceneName == other.m_SceneName &&
				   m_UISceneEntities == other.m_UISceneEntities;
		}
	};
} // namespace ecs
