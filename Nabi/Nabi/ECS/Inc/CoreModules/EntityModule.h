#pragma once
#include "Core.h"

#include "Context.h"
#include "EntityGroup.h"

namespace ecs::EntityModule
{
	[[nodiscard]] entt::entity FindFirstEntityByName(nabi::Context const& context, entt::hashed_string const entityName);
	[[nodiscard]] nabi::ECS::EntityGroup FindEntitiesByName(nabi::Context const& context, entt::hashed_string entityName);

	[[nodiscard]] entt::entity FindFirstEntityByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);
	[[nodiscard]] nabi::ECS::EntityGroup FindEntitiesByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);

	void ForeachEntityChild(nabi::Context& context, entt::entity const entity, std::function<bool(entt::entity const)> const& action);
	void ForeachEntityChild(nabi::Context& context, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action);

	// These functions can be used to get game singleton components easily. Game singleton components are defined 
	// in xml to be fully data driven, and so are not stored in an array like engine (core) singleton components.
	template<typename T>
	[[nodiscard]] T const& GetSingletonComponent(nabi::Context const& context)
	{
		// i'm not quite sure why i have to do it this way here, but in CollisionSystem can use a structured binding
		// is it faster to cache the singleton components? probs..? get it working first at any rate

		auto view = context.m_Registry.view<T const>();
		ASSERT(view.size() == 1u, 
			"Singleton components are expected to only be on one entity. Found the component 'T' on " << view.size() << " entities.");

		T const& singletonComponent = context.m_Registry.get<T>(view.front());
		return singletonComponent;
	}
	template<typename T>
	[[nodiscard]] inline T& GetSingletonComponent(nabi::Context& context)
	{
		return const_cast<T&>(GetSingletonComponent<T>(const_cast<nabi::Context const&>(context)));
	}
} // namespace ecs::EntityModule
