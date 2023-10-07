#pragma once
#include "Core.h"

#include <any>
#include <stack>
#include <unordered_map>

#include "ComponentBase.h"
#include "CoreComponents\UISceneComponent.h"

namespace ecs::SComp
{
	struct UIStateComponent final : public nabi::ECS::ComponentBase
	{
		std::stack<UISceneComponent> m_UIScenes;

		UIStateComponent()
			: m_UIScenes{}
		{
		}
	};

	struct UIStorageComponent final : public nabi::ECS::ComponentBase
	{
		typedef unsigned int Sticky;
		std::unordered_map<Sticky, std::any> m_Storage;

		UIStorageComponent()
			: m_Storage{}
		{
		}
	};
} // namespace ecs::SComp
