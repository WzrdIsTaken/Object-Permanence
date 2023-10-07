#pragma once
#include "Core.h"

#include "ComponentBase.h"

// The SpatialHierarchyComponent is like the EntityInfoComponent. See EntityInfoComponent.h for an explanation

namespace ecs
{
	// Settings
	struct SpatialHierarchyComponentSettings final : public nabi::ECS::ComponentSettingsBase
	{
		entt::entity m_Parent;
		std::vector<entt::entity> m_Children;
	};

	// Default Settings
	SpatialHierarchyComponentSettings const c_SpatialHierarchyComponentDefaultSettings
	{
		.m_Parent = entt::null,
		.m_Children = {}
	};

	// Component
	struct SpatialHierarchyComponent final : public nabi::ECS::ComponentBase
	{
		SpatialHierarchyComponent()
			: m_Parent(entt::null)
			, m_Children({})
		{}
		explicit SpatialHierarchyComponent(SpatialHierarchyComponentSettings const& settings)
			: m_Parent(settings.m_Parent)
			, m_Children(settings.m_Children)
		{}

		entt::entity m_Parent;
		std::vector<entt::entity> m_Children;
	};
} // namespace ecs
