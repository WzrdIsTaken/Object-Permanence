#include "Core.h"

#include "CoreModules\EntityModule.h"

#include "CoreComponents\EntityInfoComponent.h"
#include "CoreComponents\SpatialHierarchyComponent.h"

namespace ecs::EntityModule
{
	using namespace nabi::ECS;

	namespace
	{
#pragma warning( push )
#pragma warning( disable : 26495 ) // [var] is unitialized (perhaps the constexpr is confusing vs?)
		struct FindSettings final
		{
			enum class SearchType : int
			{
				Name,
				Group,
				ENUM_COUNT
			};

			static int constexpr c_FindAll = -1;

			entt::hashed_string m_FindTerm;
			SearchType m_SearchType;
			int m_SearchCount; // -1 for all
		};
#pragma warning( pop )

		EntityGroup FindEntitiyHelper(nabi::Context const& context, FindSettings const findSettings)
		{
			EntityGroup matchingEntities;

			std::function<bool(EntityInfoComponent const&, entt::hashed_string const)> findPredicate;
			switch (findSettings.m_SearchType)
			{
			case FindSettings::SearchType::Name:
				findPredicate = [](EntityInfoComponent const& entityInfoComponent, entt::hashed_string const entityName) -> bool 
				{ 
					return entityInfoComponent.m_EntityName == entityName;  
				};
				break;
			case FindSettings::SearchType::Group:
				findPredicate = [](EntityInfoComponent const& entityInfoComponent, entt::hashed_string const entityGroup) -> bool
				{ 
					return entityInfoComponent.m_EntityGroup == entityGroup; 
				};
				break;
			default:
				ASSERT_FAIL_FATAL("Using an unexpected FindSettings::SearchType!");
				break;
			}

			int entitiesFound = 0;
			auto view = context.m_Registry.view<ecs::EntityInfoComponent const>();

			for (auto const [entity, entityInfoComponent] : view.each())
			{
				bool const entityMatchesFindTerm = findPredicate(entityInfoComponent, findSettings.m_FindTerm);
				if (entityMatchesFindTerm)
				{
					matchingEntities.AddEntity(entity);
					++entitiesFound;

					if (entitiesFound == findSettings.m_SearchCount)
					{
						break;
					}
				}
			}

			return matchingEntities;
		}
	}

	entt::entity FindFirstEntityByName(nabi::Context const& context, entt::hashed_string const entityName)
	{
		FindSettings findSettings;
		findSettings.m_FindTerm = entityName;
		findSettings.m_SearchType = FindSettings::SearchType::Name;
		findSettings.m_SearchCount = 1;

		EntityGroup const entityGroup = FindEntitiyHelper(context, findSettings);
		EntityGroup::Group const& entities = entityGroup.GetGroup();

		ASSERT(entities.empty() || entities.size() == 1u, "FindFirstEntityByName trying to return more than one entity!");
		CONDITIONAL_LOG(entities.empty(), LOG_PREP, LOG_INFO, LOG_CATEGORY_ECS, "FindFirstEntityByName didn't return any results!", LOG_END);

		entt::entity firstEntityByName = entt::null;
		if (!entities.empty())
		{
			firstEntityByName = entities.front();
		}

		return firstEntityByName;
	}

	EntityGroup FindEntitiesByName(nabi::Context const& /*context*/, entt::hashed_string /*entityName*/)
	{
		FUNCTION_NOT_IMPLEMENTED
		return EntityGroup();
	}

	entt::entity FindFirstEntityByGroup(nabi::Context const& /*context*/, entt::hashed_string const /*entityGroupName*/)
	{
		FUNCTION_NOT_IMPLEMENTED
		return entt::entity();
	}

	EntityGroup FindEntitiesByGroup(nabi::Context const& /*context*/, entt::hashed_string const /*entityGroupName*/)
	{
		FUNCTION_NOT_IMPLEMENTED
		return EntityGroup();
	}

	void ForeachEntityChild(nabi::Context& context, entt::entity const entity, std::function<bool(entt::entity const)> const& action)
	{
		SpatialHierarchyComponent& spatialHierarchyComponent = context.m_Registry.get<SpatialHierarchyComponent>(entity);
		ForeachEntityChild(context, spatialHierarchyComponent.m_Children, action);
	}

	void ForeachEntityChild(nabi::Context& /*context*/, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action)
	{
		for (entt::entity child : children)
		{
			bool const continueLooping = action(child);
			if (!continueLooping)
			{
				break;
			}
		}
	}
} // namespace ecs::EntityModule
