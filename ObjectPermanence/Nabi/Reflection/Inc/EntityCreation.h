#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "MetaECSTypes.h"

namespace nabi::Reflection::Creation
{
	// A collection of functions shared between EntityCreator and XmlParser for creating entities at runtime

	// Xml logging bool
#ifdef USE_DEBUG_UTILS
	/*
		This is a bit jank, but originally these functions were in XmlParser and I want to keep the nice logging about entity creation 
		I had going there. The functions could take in a debug only bool or something, but I feel like that would just result in a load
		of ifdefs (well, a load more...). 

		This way I can just collapse a couple of these preprocessor blocks and forget about it!

		If anyone knows a better way of doing this, I'd love to know!
	*/

	inline bool g_XmlParserLog = false;
#endif // ifdef USE_DEBUG_UTILS

	/// <summary>
	/// Foreach property on each component, check if the property is being overriden and assign the new value to it
	/// </summary>
	/// <param name="componentData">- The components on the entity</param>
	/// <param name="propertyComponentRef">- Which component the property override is referencing</param>
	/// <param name="propertyId">- Which property the property override is referencing</param>
	/// <param name="propertyValue">- The new value of the property</param>
	/// <param name="propertyDataOverride">- If the property cannot be found on the component, what property data to add [optional]</param>
	void OverrideEntityComponents(std::vector<ComponentData>& componentData,
		entt::hashed_string const propertyComponentRef, entt::hashed_string const propertyId, entt::hashed_string const propertyValue,
		PropertyData const* const propertyDataOverride) NABI_NOEXCEPT;
	/// <summary>
	/// Foreach ComponentData in [entityComponents], goes through the process of resolving them and assigning them to [entity]
	/// </summary>
	/// <param name="entityComponents">- The list of components to resolve and assign</param>
	/// <param name="registery">- A reference to the registery</param>
	/// <param name="entity">- The entity to assign the components to</param>
	void ResolveEntityComponents(std::vector<ComponentData> const& entityComponents, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT;
	/// <summary>
	/// Creates a meta component from component data
	/// </summary>
	/// <param name="componentData">- The data for the component</param>
	/// <param name="componentId">- The component's id. For looking up its type descriptor</param>
	/// <returns>The meta component</returns>
	[[nodiscard]] entt::meta_any ResolveComponent(ComponentData const& componentData, std::string_view const componentId) NABI_NOEXCEPT;
	/// <summary>
	/// Resolves properties on a system or entity. Ie, this process takes the string property id, value and assigns the specified value to the specified meta property.
	/// </summary>
	/// <param name="data">- The system or entity node on which the propertie reside</param>
	/// <param name="metaObject">- The object to which the properties belong to</param>
	void ResolveProperties(MetaECSTypeData const& data, entt::meta_any& metaObject) NABI_NOEXCEPT;
	/// <summary>
	/// Assigns the specified property to the passed in metaObject.
	/// </summary>
	/// <param name="metaComponent">- The meta object to assign the property to</param>
	/// <param name="propertyName">- The name of the property</param>
	/// <param name="propertyValue">- The value of the property</param>
	void ResolveProperty(entt::meta_any& metaObject, std::string_view const propertyName, std::string_view const propertyValue) NABI_NOEXCEPT;

	/// <summary>
	/// Adds the EntityInfoComponent to an entity. This component contains some basic data about the entity.
	/// </summary>
	/// <param name="registey">- A reference to the registery, required for assigning the component to the entity</param>
	/// <param name="entity">- The entity to assign the component to</param>
	/// <param name="entityGroupHash">- The hash of the entity group (hash as its already in this form from ParseEntities</param>
	/// <param name="entityName">- The entity's name</param>
	void AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity, 
			                            entt::hashed_string const& entityGroupHash, std::string_view const entityName) NABI_NOEXCEPT;
	/// <summary>
	/// Adds the SpatialHierarchyComponent to an entity. This component contains the component's parent and the entitie's children
	/// </summary>
	/// <param name="registey">- A reference to the registery</param>
	/// <param name="entity">- The entity to assign the component to</param>
	void AddSpatialHierarchyComponentToEntity(entt::registry& registey, entt::entity const parent) NABI_NOEXCEPT;

	/// <summary>
	/// Calls the Assign function on a component's Reflector, which emplaces the component to the entity
	/// </summary>
	/// <param name="metaComponent">- The component to assign to the entity</param>
	/// <param name="entity">- The entity</param>
	/// <param name="registery">- A reference to the registery</param>
	void AssignComponentToRegistery(entt::meta_any& metaComponent, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT;
} // namespace nabi::Reflection::Creation
