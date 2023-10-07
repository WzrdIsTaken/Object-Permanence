#include "EngineCore.h"

#include "EntityCreator.h"

#include "CoreComponents/EntityInfoComponent.h"
#include "DebugUtils.h"
#include "ECSUtils.h"
#include "EntityCreation.h"
#include "EntityPropertyList.h"

namespace nabi::ECS
{
	EntityCreator::EntityCreator(entt::registry& registry) NABI_NOEXCEPT
		: m_Registry(registry)
	{
	}

	void EntityCreator::AssignEntityTemplateStore(EntityTemplateStore const&& entityTemplateStore) NABI_NOEXCEPT
	{
		m_EntityTemplateStore = entityTemplateStore;
	}

	void EntityCreator::AssignEntityGroupStore(EntityGroupStore const&& entityGroupStore) NABI_NOEXCEPT
	{
		m_EntityGroupStore = entityGroupStore;
	}

	entt::entity EntityCreator::CreateEntity(EntityCreator::EntityCreationSettings const* const entityCreationSettingsPtr) NABI_NOEXCEPT
	{
		EntityCreationSettings entityCreationSettings = {};
		Reflection::EntityTemplateData* entityTemplate = nullptr;

		if (entityCreationSettingsPtr != nullptr)
		{
			entityCreationSettings = *entityCreationSettingsPtr;

			if (entityCreationSettingsPtr->m_EntityTemplateName != "")
			{
				std::string const& entityTemplateName = entityCreationSettings.m_EntityTemplateName;
				ASSERT_FATAL(m_EntityTemplateStore.find(entityTemplateName) != m_EntityTemplateStore.end(),
					"Trying to create an entity, " << WRAP(entityTemplateName, "'") << ", which doesn't exist in the entity store!");

				entityTemplate = &m_EntityTemplateStore.at(entityTemplateName.data());
			}
		}

		std::string& entityName = entityCreationSettings.m_EntityName;
		entt::entity const entity = m_Registry.create();

		if (entityCreationSettings.m_AppendUUID)
		{
			entityName += "_" + nabi::ECSUtils::GetEntityUUIDAsString(entity);
		}

		if (entityCreationSettings.m_EntityOverriddenProperties != nullptr)
		{
			EntityPropertyList const& overridenProperties = *entityCreationSettings.m_EntityOverriddenProperties;
			ASSERT_FATAL(entityTemplate, "Trying to override an entity's component's properties but the entityTemplate is null!");
			ResolveEntityTemplateComponents(*entityTemplate, overridenProperties);
		}

		if (entityTemplate != nullptr)
		{
			Reflection::Creation::ResolveEntityComponents(entityTemplate->m_Components, m_Registry, entity);
		}
		Reflection::Creation::AddEntityInfoComponentToEntity(m_Registry, entity, entityCreationSettings.m_EntityGroup, entityName);
		Reflection::Creation::AddSpatialHierarchyComponentToEntity(m_Registry, entity);

		return entity;
	}

	bool EntityCreator::DestroyEntity(entt::entity& entity) NABI_NOEXCEPT
	{
		bool const entityExists = m_Registry.valid(entity);

		if (entityExists)
		{
			m_Registry.destroy(entity);
			entity = entt::null;
		}
		else
		{
			ASSERT_FAIL("Trying to destroy an entity which isn't in the registry!");
		}

		return entityExists;
	}

	entt::entity EntityCreator::CloneEntity(entt::entity const entityToClone) NABI_NOEXCEPT
	{
		// Again, note the warning in the header file! May cause unexpected behaviour when dealing with pointers and the like.

		entt::entity const clone = m_Registry.create();

		for (auto&& currentStorage : m_Registry.storage())
		{
			if (auto& storage = currentStorage.second; storage.contains(entityToClone))
			{
				storage.emplace(clone, storage.get(entityToClone));
			}
		}

		ecs::EntityInfoComponent entityInfoComponent = m_Registry.get<ecs::EntityInfoComponent>(clone);
		std::string const clonedName = std::string(entityInfoComponent.m_EntityName.data()) + std::string("_clone");
		entityInfoComponent.m_EntityName = entt::hashed_string(clonedName.c_str());

		return clone;
	}

	EntityGroup EntityCreator::CreateEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT
	{
		EntityGroup returnEntityGroup = {};

		auto entityGroup = m_EntityGroupStore.find(entityGroupName);
		if (entityGroup != m_EntityGroupStore.end())
		{
			std::vector<Reflection::EntityData> const& entitiesInGroup = entityGroup->second;
			returnEntityGroup.GetGroup().reserve(entitiesInGroup.size());

			EntityCreationSettings entityCreationSettings = {};
			// Could be useful to extract the entity's name one day?
			entityCreationSettings.m_EntityGroup = entt::hashed_string(entityGroupName.c_str());

			for (Reflection::EntityData const& entityToCreate : entitiesInGroup)
			{
				entt::entity const entity = CreateEntity(&entityCreationSettings);
				std::vector<Reflection::ComponentData> const& entityComponents = entityToCreate.m_Components;
				Reflection::Creation::ResolveEntityComponents(entityToCreate.m_Components, m_Registry, entity);

				returnEntityGroup.AddEntity(entity);
			}
		}
		else
		{
			ASSERT_FAIL("Trying to create an entity group which doesn't exist!");
		}

		return returnEntityGroup;
	}

	void EntityCreator::CreateEntityGroup(EntityGroup const& /*entityGroup*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED
	}

	bool EntityCreator::IsEntityGroupLoaded(std::string const& entityGroupName) const NABI_NOEXCEPT
	{
		entt::hashed_string const hashedGroupName = entt::hashed_string(entityGroupName.c_str());
		bool groupIsLoaded = false;

		for (auto [entity, entityInfoComponent] : m_Registry.view<ecs::EntityInfoComponent const>().each())
		{
			if (entityInfoComponent.m_EntityGroup == hashedGroupName)
			{
				groupIsLoaded = true;
				break;
			}
		}

		return groupIsLoaded;
	}

	bool EntityCreator::IsEntityGroupLoaded(EntityGroup const& /*entityGroup*/) const NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED
		return false;
	}

	bool EntityCreator::DestroyEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT
	{
		entt::hashed_string const hashedGroupName = entt::hashed_string(entityGroupName.c_str());
		bool validGroup = false;

		m_Registry.view<ecs::EntityInfoComponent const>()
			.each([&](entt::entity entity, auto& entityInfoComponent) -> void
				{
					if (entityInfoComponent.m_EntityGroup == hashedGroupName)
					{
						DestroyEntity(entity);
						validGroup = true;
					}
				});

		if (!validGroup)
		{
			ASSERT_FAIL("Trying to destroy an entity group which has no entities or doesn't exist!");
		}
		return validGroup;
	}

	bool EntityCreator::DestroyEntityGroup(EntityGroup& /*entityGroup*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED
		return false;
	}

	size_t EntityCreator::GetEntityStoreSize() const NABI_NOEXCEPT
	{
		return m_EntityTemplateStore.size();
	}

	size_t EntityCreator::GetEntityGroupStoreSize() const NABI_NOEXCEPT
	{
		return m_EntityGroupStore.size();
	}

	void EntityCreator::ResolveEntityTemplateComponents(Reflection::EntityTemplateData& entityTemplateData, EntityPropertyList const& entityOverriddenProperties) NABI_NOEXCEPT
	{
		/*
		  bloody hell
		  it is all compairing / assigning ints under the hood
		  but still this is kinda rough
		  O(10000000)
		*/

		// Foreach property in the property list
		for (EntityPropertyList::PropertyComponentPair const& propertyComponentPair : entityOverriddenProperties.GetOverridenProperties())
		{
			Reflection::Creation::OverrideEntityComponents(
				entityTemplateData.m_Components,                  // Components on the entity
				propertyComponentPair.m_ComponentHash,            // Component which the property override is referencing
				propertyComponentPair.m_PropertyOverride.m_Id,    // Property Id which the property override is refencing
				propertyComponentPair.m_PropertyOverride.m_Value, // The new value of the property
				&propertyComponentPair.m_PropertyOverride         // If the property doesn't exist on the component, what to add
			);
		}
	}
} // namespace nabi::ECS
