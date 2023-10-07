#pragma once
#include "Core.h"

#include "entt.h"

#include "ComponentBase.h"

// Contains some basic infomation about an entity.
// No need to reflect, as its always added anyway.

/*
  This component works a bit differently to the others, as the data it uses isn't assigned (explicitly) in xml.
  It takes in a settings struct as an intermediary. I planned for all components to do this, but then realised it wasn't
  actually needed as all other component's values are only set in xml. However, this component doesn't work like that.

  The intermediary settings struct means that if another member is added to EntityInfoComponent and the constructor is changed,
  all occurances of it do not need to be updated. Of course this doesn't really matter for this component, as it is only used
  in one place - but you can see how it would be useful in an ecs without a backend reflection system.

  This component, and any other core ecs blueprints, live in the 'ecs' namespace to make them more closely linked to the game's
  (eventual) 'ecs' code.
*/

namespace ecs
{
	// Settings
	struct EntityInfoComponentSettings final : public nabi::ECS::ComponentSettingsBase
	{
		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;
	};

	// Default Settings
	EntityInfoComponentSettings const c_EntityInfoComponentDefaultSettings
	{
		.m_EntityGroup = entt::hashed_string(),
		.m_EntityName = entt::hashed_string()
	};

	// Component
	struct EntityInfoComponent final : public nabi::ECS::ComponentBase
	{
		EntityInfoComponent()
			: m_EntityGroup(entt::hashed_string())
			, m_EntityName(entt::hashed_string())
		{}
		explicit EntityInfoComponent(EntityInfoComponentSettings const& settings)
			: m_EntityGroup(settings.m_EntityGroup)
			, m_EntityName(settings.m_EntityName)
		{}

		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;
	};
} // namespace ecs
