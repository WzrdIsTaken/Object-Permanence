#include "EngineCore.h"

#include "EntityCreation.h"

#include "CoreComponents/EntityInfoComponent.h"
#include "CoreComponents/SpatialHierarchyComponent.h"
#include "DebugUtils.h"
#include "ReflectionGlobals.h"
#include "ReflectionHelpers.h"
#include "StringConverter.h"
#include "StringStore.h"

namespace nabi::Reflection::Creation
{
	void OverrideEntityComponents(std::vector<ComponentData>& componentData,
		entt::hashed_string const propertyComponentRef, entt::hashed_string const propertyId, entt::hashed_string const propertyValue,
		PropertyData const* const propertyDataOverride) NABI_NOEXCEPT
	{
		// Loop through all of the component's properties
		for (ComponentData& componentData : componentData)
		{
			// If the property is referencing this component
			if (componentData.m_Id == propertyComponentRef)
			{
				// Foreach property on that component
				for (PropertyData& propertyData : componentData.m_Properties)
				{
					// If we find the property on one of the component data, override it
					if (propertyData.m_Id == propertyId)
					{
						propertyData.m_Value = propertyValue;
						goto OverrideEntityComponentsExit;
					}
				}

				// If we didn't find it, but the property links to this component, it just means that the property has not been set in xml
				// and will be default initialized. To override that default initialization, add the property data to the component data
				if (propertyDataOverride == nullptr)
				{
					// If there is no set override, create a new one
					PropertyData additionalPropertyData;
					additionalPropertyData.m_Id = propertyId;
					additionalPropertyData.m_Value = propertyValue;

					componentData.m_Properties.push_back(additionalPropertyData);
				}
				else
				{
					// Else we can save the construction and just add the one which already exists
					componentData.m_Properties.emplace_back(*propertyDataOverride);
				}

				goto OverrideEntityComponentsExit;
			}
		}

		ASSERT_FAIL("The property is referencing a component which doesn't exist!");

	OverrideEntityComponentsExit:
#ifdef USE_DEBUG_UTILS
		if (g_XmlParserLog)
		{
			LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION, "Found a property node with id " << WRAP(propertyId.data(), "'")
				<< " overriding a value on " << WRAP(propertyComponentRef.data(), "'") << " with value " << WRAP(propertyValue.data(), "'"), LOG_END);
		}
#endif // ifdef USE_DEBUG_UTILS
		return; // Because without the preprocessor block code, this would be a lable then end of function
	}

	void ResolveEntityComponents(std::vector<ComponentData> const& entityComponents, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT
	{
		// Spin through the entity components, resolve them and add all them all to the registery
		for (ComponentData const& component : entityComponents)
		{
			// Get the component's ID and resolve it
			std::string_view const componentId = component.m_Id.data();
			entt::meta_any metaComponent = ResolveComponent(component, componentId);

#ifdef USE_DEBUG_UTILS
			if (g_XmlParserLog)
			{
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION, "Found a component on the entity with id " << WRAP(componentId, "'"), LOG_END);
			}
#endif // ifdef USE_DEBUG_UTILS

			// Assign the component to the registery
			AssignComponentToRegistery(metaComponent, registery, entity);
		}
	}

	entt::meta_any ResolveComponent(ComponentData const& componentData, std::string_view const componentId) NABI_NOEXCEPT
	{
		// Resolve the component
		entt::meta_any metaComponent = ReflectionHelpers::ConstructMetaObject(componentId);

		// Iterate through all of the component's nodes children - these will be the component's properties 
		ResolveProperties(componentData, metaComponent);

		// Return the created component
		return metaComponent;
	}

	void ResolveProperties(MetaECSTypeData const& data, entt::meta_any& metaObject) NABI_NOEXCEPT
	{
		// Spin through all the system/components properties
		for (PropertyData const& propertyNode : data.m_Properties)
		{
			// Get the property's infomation
			std::string_view const propertyId = propertyNode.m_Id.data();
			std::string_view const propertyValue = propertyNode.m_Value.data();

#ifdef USE_DEBUG_UTILS
			if (g_XmlParserLog)
			{
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION, "Found a property with id "
					<< WRAP(propertyId, "'") << " and value " << WRAP(propertyValue, "'"), LOG_END);
			}
#endif // ifdef USE_DEBUG_UTILS

			// Resolve the property
			ResolveProperty(metaObject, propertyId, propertyValue);
		}
	}

	void ResolveProperty(entt::meta_any& metaObject, std::string_view const propertyName, std::string_view const propertyValue) NABI_NOEXCEPT
	{
		// Get the properties name hash and data
		entt::hashed_string const metaPropertyHash = entt::hashed_string(propertyName.data());
		entt::meta_data const metaPropertyData = metaObject.type().data(metaPropertyHash);

		// If you get an error on this next line, double check that all your property names in the xml are correct!
		std::string_view const propertyTypeName = metaPropertyData.type().info().name();
		std::string propertyTypeNameAsStr = std::string(propertyTypeName);

		// Get the property's type hash
		StringConverter::ExtractTypeName(propertyTypeNameAsStr);
		entt::hashed_string const propertyTypeHash = entt::hashed_string(propertyTypeNameAsStr.c_str());

		// Convert the property from string, and assign it to the meta object's data
		StringConverter::ConvertFromString(propertyValue.data(), propertyTypeHash, metaObject, metaPropertyData);
	}

	void AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity,
		entt::hashed_string const& entityGroupHash, std::string_view entityName) NABI_NOEXCEPT
	{
		std::string const& storedEntityGroup = StringStore::Instance()->Add(entityGroupHash.data());
		std::string const& storedEntityName = StringStore::Instance()->Add(entityName);

		auto entityInfoComponentSettings = ecs::c_EntityInfoComponentDefaultSettings;
		entityInfoComponentSettings.m_EntityGroup = entt::hashed_string(storedEntityGroup.c_str());
		entityInfoComponentSettings.m_EntityName = entt::hashed_string(storedEntityName.c_str());

		registey.emplace<ecs::EntityInfoComponent>(entity, entityInfoComponentSettings);
	}

	void nabi::Reflection::Creation::AddSpatialHierarchyComponentToEntity(entt::registry& registey, entt::entity const parent) NABI_NOEXCEPT
	{
		auto spatialHierarchyComponentSettings = ecs::c_SpatialHierarchyComponentDefaultSettings;
		spatialHierarchyComponentSettings.m_Parent = parent;
		spatialHierarchyComponentSettings.m_Children = {};

		registey.emplace<ecs::SpatialHierarchyComponent>(parent, spatialHierarchyComponentSettings);
	}

	void AssignComponentToRegistery(entt::meta_any& metaComponent, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT
	{
		// Find the assign function and call it on the specified entity
		if (entt::meta_func const assign = metaComponent.type().func(ReflectionGlobals::c_AssignFunctionName))
		{
			assign.invoke(metaComponent, entt::forward_as_meta(metaComponent), entt::forward_as_meta(registery), entity);
		}
		else
		{
			ASSERT_FAIL("The component " << WRAP(metaComponent.type().info().name(), "'") << " does not have an 'Assign' method!");
		}
	}
} // namespace nabi::Reflection
