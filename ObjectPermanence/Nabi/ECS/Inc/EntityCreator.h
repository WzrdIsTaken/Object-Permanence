#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "ECSGlobals.h"
#include "EntityGroup.h"
#include "MetaECSTypes.h"
#include "TypeUtils.h"

namespace nabi::ECS
{
	class EntityPropertyList;
} // namespace nabi::ECS

namespace nabi::ECS
{
	/// <summary>
	/// Used to create entities at runtime
	/// (I know that the xml parser is technically creating runtime entities... but this is runtimeruntime! :D)
	/// 
	/// Ngl a lot of things this class does don't seem very efficient / jank... this might be a point of optimisation / improvement in the future
	/// </summary>
	class EntityCreator final
	{
	public:
		typedef std::unordered_map<std::string, Reflection::EntityTemplateData> EntityTemplateStore;   // (entity template name - template)
		typedef std::unordered_map<std::string, std::vector<Reflection::EntityData>> EntityGroupStore; // (entity group name - vector of entity templates)  

		struct EntityCreationSettings final
		{
			std::string m_EntityTemplateName = "";
			entt::hashed_string m_EntityGroup = ECSGlobals::c_DefaultEntityGroup;
			mutable std::string m_EntityName = "Entity";
			EntityPropertyList* m_EntityOverriddenProperties = nullptr; // (optional)
			bool m_AppendUUID = true;
		}; // We can't have a c_DefaultEntityCreationSettings for this because of the dynamic allocations it can't use an in-class initializer

		EntityCreator(entt::registry& registry) NABI_NOEXCEPT;
		void AssignEntityTemplateStore(EntityTemplateStore const&& entityTemplateStore) NABI_NOEXCEPT;
		void AssignEntityGroupStore(EntityGroupStore const&& entityGroupStore) NABI_NOEXCEPT;

		/// <summary>
		/// Creates an entity based off the passed in EntityCreationSettings
		/// </summary>
		/// <param name="entityCreationSettingsPtr">- The settings defining how to create the entity. Pass in nullptr for default settings</param>
		entt::entity CreateEntity(EntityCreationSettings const* const entityCreationSettingsPtr = nullptr) NABI_NOEXCEPT;
		/// <summary>
		/// Destroys an entity and recycles its identifier
		/// </summary>
		/// <param name="entity">- The entity to delete</param>
		/// <returns>If the entity was successfully deleted</returns>
		bool DestroyEntity(entt::entity& entity) NABI_NOEXCEPT;

		/// <summary>
		/// Copies all of the components from one entity to another. 
		/// WARNING: If any of the components contain pointers, for example a BufferComponent, they will be copied as well and then point at the same thing!
		/// </summary>
		/// <param name="entityToClone">The entities whos components will be copied</param>
		/// <returns></returns>
		entt::entity CloneEntity(entt::entity const entityToClone) NABI_NOEXCEPT;

		// Creates an entity group based off the groups name
		EntityGroup CreateEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT;
		void CreateEntityGroup(EntityGroup const& entityGroup) NABI_NOEXCEPT;

		// Checks if any entities part of an entity group exist
		bool IsEntityGroupLoaded(std::string const& entityGroupName) const NABI_NOEXCEPT;
		bool IsEntityGroupLoaded(EntityGroup const& entityGroup) const NABI_NOEXCEPT;

		// Deletes an entity group based off the groups name
		bool DestroyEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT;
		bool DestroyEntityGroup(EntityGroup& entityGroup) NABI_NOEXCEPT;

		// Returns how many entity templates exist in m_EntityTemplateStore
		[[nodiscard]] size_t GetEntityStoreSize() const NABI_NOEXCEPT;
		// Returns how many entity groups exist in m_EntityGroupStore
		[[nodiscard]] size_t GetEntityGroupStoreSize() const NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(EntityCreator)

		/// <summary>
		/// Foreach property in [entityOverriddenProperties] checks to see if [entityTemplateData] has a corresponding property which can be overriden
		/// </summary>
		/// <param name="entityTemplateData">- The template to override properties on (note! even though this is a reference, *don't* pass
		///                                    in a refence from m_EntityTemplateStore or the like. Take a copy first! Or the base template will be overriden </param>
		/// <param name="entityOverriddenProperties">- A EntityPropertyList of which properties to try and override on the template</param>
		void ResolveEntityTemplateComponents(Reflection::EntityTemplateData& entityTemplateData, EntityPropertyList const& entityOverriddenProperties) NABI_NOEXCEPT;

		EntityTemplateStore m_EntityTemplateStore;
		EntityGroupStore m_EntityGroupStore;
		entt::registry& m_Registry;
	};
} // namespace nabi::ECS
