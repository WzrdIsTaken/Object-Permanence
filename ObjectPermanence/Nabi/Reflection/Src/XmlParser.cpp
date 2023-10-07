#include "EngineCore.h"

#include "XmlParser.h"

#include "pugixml.hpp"

#include "BuildUtils.h"
#include "Context.h"
#include "DebugUtils.h"
#include "EntityCreation.h"
#include "MetaObjectLookup.h"
#include "ReflectionGlobals.h"
#include "ReflectionHelpers.h"
#include "StringConverter.h"
#include "StringStore.h"
#include "TestUtils.h"

namespace nabi::Reflection
{
	void XmlParser::ParseXml(std::string_view const routeDocPath, nabi::Context& context, MetaObjectLookup* const systemsLookup) NABI_NOEXCEPT
	{
		// Load the route document
		pugi::xml_document const routeDoc = LoadDocument(routeDocPath);
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, "Loaded a route document " << WRAP(routeDocPath, "'") << " successfully!", LOG_END);

		// Cache the context's registry
		entt::registry& registry = context.m_Registry;

		// Enable Creation logging (see EntityCreator.h for an explanation of this jankness)
#ifdef USE_DEBUG_UTILS
		Creation::g_XmlParserLog = true;
#endif // ifdef USE_DEBUG_UTILS

		for (pugi::xml_node const pathNode : routeDoc.child(c_RouteDocAttribute.c_str()))
		{
			// Go through each of the path nodes and load the doc associated with them
			std::string_view const path = pathNode.attribute(c_ValueAttribute.c_str()).value();
			pugi::xml_document const dataDoc = LoadDocument(path);
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, "Loaded a data document " << WRAP(path, "'") << " successfully!", LOG_END);

			// Take a peak at the type of that document
			std::string_view const docType = dataDoc.first_child().name();

			// Depending on the document's type, call the relevant parse function
			if (docType == c_RouteDocAttribute)
			{
				ParseXml(path, context, systemsLookup);
			}
			else if (docType == c_SingletonGroupAttribute)
			{
				ParseSingletons(dataDoc);
			}
			else if (docType == c_SystemGroupAttribute)
			{
				ParseSystems(dataDoc, context, systemsLookup);
			}
			else if (docType == c_EntityGroupAttribute)
			{
				ParseEntities(dataDoc, registry);
			}
			else
			{
				ASSERT_FAIL("The document type isn't valid!");
			}
		}

		// Disable Creation logging
#ifdef USE_DEBUG_UTILS
		Creation::g_XmlParserLog = false;
#endif // ifdef USE_DEBUG_UTILS
	}

	pugi::xml_document XmlParser::LoadDocument(std::string_view const docPath) const NABI_NOEXCEPT
	{
		pugi::xml_document doc;
		pugi::xml_parse_result const result = doc.load_file(docPath.data());

		ASSERT_FATAL(result, "The document " << WRAP(docPath, "'") << " could not be found!");
		return doc;
	}

	void XmlParser::ParseSingletons(pugi::xml_document const& /*doc*/) const NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED

		// I dont know how singletons will work
		// And I don't need them right now (or ever??), so:
		// - If I have a sudden brainwave I will make them
		// - Else I will think about them when I actually need to 
	}

	void XmlParser::ParseSystems(pugi::xml_document const& doc, nabi::Context& context, MetaObjectLookup* const systemsLookup) const NABI_NOEXCEPT
	{
		// Get the system group's id
		std::string const& systemGroupId = StringStore::Instance()->Add(doc.first_child().attribute(c_IdAttribute.c_str()).value());
		entt::hashed_string const systemGroupIdHash = entt::hashed_string(systemGroupId.c_str());
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, "Found a system group with id " << WRAP(systemGroupId, "'"), LOG_END);

		// Iterate through all of the systems in the group
		for (pugi::xml_node const systemNode : doc.child(c_SystemGroupAttribute.c_str()))
		{
			// Check if the system is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(systemNode))
			{
				// Get the system's id
				std::string const& systemId = StringStore::Instance()->Add(systemNode.attribute(c_IdAttribute.c_str()).value());
				entt::hashed_string const systemIdHash = entt::hashed_string(systemId.c_str());
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << LOG_CATEGORY_REFLECTION, "Created a system with id " << WRAP(systemId, "'"), LOG_END);

				// Construct the system
 
				// I have no idea why, but this function was the cause of all my pain. It works in an isolated example, but not within this flow?? idk dude, im just glad i found that bug				
				//entt::meta_any metaSystem = ReflectionHelpers::ConstructMetaObject(systemIdHash, entt::forward_as_meta(context), systemIdHash, systemGroupIdHash);

				entt::meta_type const systemType = entt::resolve(systemIdHash);
				entt::meta_any metaSystem = systemType.construct(entt::forward_as_meta(context), systemIdHash, systemGroupIdHash);

				// Construct the data for the ststem
				SystemData const systemData = CreateECSTypeData(systemNode);

				// Resolve any properties on the system
				Creation::ResolveProperties(systemData, metaSystem);

				// Add the system to the system's lookup map
				if (systemsLookup != nullptr)
				{
					systemsLookup->AddObject(systemId.data(), std::move(metaSystem));
				}
			}
		}
	}

	void XmlParser::ParseEntities(pugi::xml_document const& doc, entt::registry& registery) NABI_NOEXCEPT
	{
		// Get the entity group's id
		std::string_view const entityGroupId = doc.first_child().attribute(c_IdAttribute.c_str()).value();
		entt::hashed_string const entityGroupIdHash = entt::hashed_string(entityGroupId.data());
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, "Found a entity group with id " << WRAP(entityGroupId, "'"), LOG_END);

		// Stores the entity node if it is marked for delayed creation
		std::vector<pugi::xml_node> delayedCreationEntities; // i would like to make this a std::reference_wrapper, but something funcky is going on when i use it

		// Loop through all of the entities in the group
		for (pugi::xml_node entityNode : doc.child(c_EntityGroupAttribute.c_str()))
		{
			// Check if the entity is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(entityNode))
			{
				// Check if the entity is a template or not
				bool const isEntityTemplate = entityNode.name() == c_EntityTemplateAttribute;
				if (isEntityTemplate)
				{
					ResolveEntityTemplate(entityNode);
				}
				else
				{
					// Check if the entity is marked for delayed creation
					bool isEntityMarkedForDelayedCreation = false;
					if (auto const attribute = entityNode.attribute(c_DelayCreationAttribute.c_str()); 
						attribute != nullptr && StringConverter::FromString<bool>(attribute.value()))
					{
						isEntityMarkedForDelayedCreation = true;
					}

					// Resolve the entity accordingly
					if (isEntityMarkedForDelayedCreation)
					{
						delayedCreationEntities.push_back(entityNode);
					}
					else
					{
						ResolveEntity(entityNode, entityGroupIdHash, registery);
					}
				}
			}
		}

		// Create any delayed creation entities
		if (!delayedCreationEntities.empty())
		{
			for (pugi::xml_node entityNode : delayedCreationEntities)
			{
				ResolveEntity(entityNode, entityGroupIdHash, registery);
			}
		}
	}

	void XmlParser::ResolveEntityTemplate(pugi::xml_node const& entityTemplateNode) NABI_NOEXCEPT
	{
		// Get the entity template's name
		std::string const entityTemplateName = entityTemplateNode.attribute(c_IdAttribute.c_str()).value();
		LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << LOG_CATEGORY_REFLECTION, "Found an entity template with name " << WRAP(entityTemplateName, "'"), LOG_END);
		ASSERT(m_EntityTemplates.find(entityTemplateName) == m_EntityTemplates.end(), "Adding an entity template with an id which is currently in use!");

		// Create an entry in m_EntityTemplates and get a reference to the templates components
		EntityTemplateData entityTemplateData;
		entityTemplateData.m_Id = entt::hashed_string(entityTemplateName.data());
		entityTemplateData.m_Components = {};

		m_EntityTemplates.insert({ entityTemplateName, entityTemplateData });
		std::vector<ComponentData>& entityTemplateComponents = m_EntityTemplates.at(entityTemplateName).m_Components;

		// Check if the entity template is a template of something else
		pugi::xml_attribute const entityTemplate = entityTemplateNode.attribute(c_TemplateAttribute.c_str());
		if (entityTemplate)
		{
			// Find the entity template's template
			pugi::char_t const* const entityTemplateParentName = entityTemplate.value();
			LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION, "It inherits from " << WRAP(entityTemplateParentName, "'"), LOG_END);

			ASSERT_FATAL(m_EntityTemplates.find(entityTemplateParentName) != m_EntityTemplates.end(),
				"A template with the id " << WRAP(entityTemplateParentName, "'") << " does not exist in m_EntityTemplates!");

			// Assign all the parents components to this entity template
			std::vector<ComponentData> const& parentTemplateComponents = m_EntityTemplates.at(entityTemplateParentName).m_Components;
			for (ComponentData const& component : parentTemplateComponents)
			{
				entityTemplateComponents.push_back(component);
			}
		}
		
		// Resolve the components and properties on *this* entity template
		ResolveComponentOrPropertyNode(entityTemplateNode, entityTemplateComponents);
	}

	void XmlParser::ResolveEntity(pugi::xml_node const& entityNode, entt::hashed_string const entityGroupIdHash, entt::registry& registery) NABI_NOEXCEPT
	{
		// Create an entity
		entt::entity const entity = registery.create();
		std::string_view const entityId = entityNode.attribute(c_IdAttribute.c_str()).value();
		LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << LOG_CATEGORY_REFLECTION, "Created an entity with name " << WRAP(entityId, "'"), LOG_END);

		// Create a collection of all the entities components
		std::vector<ComponentData> entityComponents{};

		// Check if the entity has a template
		pugi::xml_attribute const entityTemplate = entityNode.attribute(c_TemplateAttribute.c_str());
		if (entityTemplate)
		{
			// If the entity has a template, add all of the template entity's components to this entity
			pugi::char_t const* const entityTemplateId = entityTemplate.value();
			ASSERT_FATAL(m_EntityTemplates.find(entityTemplateId) != m_EntityTemplates.end(),
				"The Entity Template " << entityTemplateId << "does not exist!");

			std::vector<ComponentData>& entityTemplateComponents = m_EntityTemplates.at(entityTemplateId).m_Components;
			for (ComponentData const& entityTemplateComponent : entityTemplateComponents)
			{
				entityComponents.push_back(entityTemplateComponent);
			}
		}

		// Resolve the components and properties on *this* entity
		ResolveComponentOrPropertyNode(entityNode, entityComponents);

		// Spin through the entity components, resolve them and add all them all to the registery
		Creation::ResolveEntityComponents(entityComponents, registery, entity);

		// Add the EntityInfoComponent + SpatialHierarchyComponent to the entity
		Creation::AddEntityInfoComponentToEntity(registery, entity, entityGroupIdHash, entityId);
		Creation::AddSpatialHierarchyComponentToEntity(registery, entity);

		// Add the entity to a group so that they can later be created via EntityCreator::CreateEntityGroup
		char const* entityGroupIdAsChar = entityGroupIdHash.data();
		EntityData entityData = {};
		entityData.m_Id = entt::hashed_string(entityId.data());
		entityData.m_Components = entityComponents;

		AddEntityTemplateToEntityGroupStore(entityGroupIdAsChar, entityData);
	}

	void XmlParser::ResolveEntityComponents(std::vector<ComponentData>& componentData, pugi::xml_node const& propertyNode) const NABI_NOEXCEPT
	{
		ASSERT(propertyNode.attribute(c_ComponentAttribute.c_str()), "The property node doesn't have a component ref!");

		// (we don't care about propertyComponentRef string, it isn't used in any 'game time' reflection creation)
		std::string const& propertyIdStr = StringStore::Instance()->Add(propertyNode.attribute(c_IdAttribute.c_str()).value());
		std::string const& propertyValueStr = StringStore::Instance()->Add(propertyNode.attribute(c_ValueAttribute.c_str()).value());

		// Get infomation about the property
		entt::hashed_string const propertyComponentRef = entt::hashed_string(propertyNode.attribute(c_ComponentAttribute.c_str()).value());
		entt::hashed_string const propertyId = entt::hashed_string(propertyIdStr.c_str());
		entt::hashed_string const propertyValue = entt::hashed_string(propertyValueStr.c_str());

		Creation::OverrideEntityComponents(
			componentData,        // Components on the entity
			propertyComponentRef, // Component which the property override is referencing
			propertyId,           // Property Id which the property override is refencing
			propertyValue,        // The new value of the property
			nullptr               // If the property doesn't exist on the component, what to add (no pre-existing property in this case)
		);
	}

	void XmlParser::ResolveComponentOrPropertyNode(pugi::xml_node const& entityNode, std::vector<ComponentData>& entityComponents) const NABI_NOEXCEPT
	{
		// Iterate through the entity node's children
		for (pugi::xml_node const componentOrPropertyNode : entityNode.children())
		{
			// Check if the component or property is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(componentOrPropertyNode))
			{
				// Check if the node is a property node
				bool const isPropertyNode = componentOrPropertyNode.name() == c_PropertyAttribute;
				if (isPropertyNode)
				{
					// If the node is a property node, then we want to override an assigned components property with its value
					ResolveEntityComponents(entityComponents, componentOrPropertyNode);
					continue;
				}

				// Otherwise, the node must be a component node. Create a component for this data
				ComponentData const component = CreateECSTypeData(componentOrPropertyNode);

				// Entt already doesn't allow this, but this assert is more explicit
				ASSERT(std::find(entityComponents.begin(), entityComponents.end(), component) == entityComponents.end(),
					"The entity already has a component of type " << WRAP(component.m_Id.data(), "'") << "!" << ENDLINE);

				// Push back the created component into the entity's component list
				entityComponents.push_back(component);
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION, "Found a component on the entity with id " 
					<< WRAP(component.m_Id.data(), "'"), LOG_END);
			}
		}
	}

	bool XmlParser::CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(pugi::xml_node const& node) const NABI_NOEXCEPT
	{
		MAKE_LOGIC_TESTABLE // (test in ReflectionDebugAttributeTests.cpp)
		{
			pugi::xml_attribute const debugAttribute = node.attribute(c_DebugAttribute.c_str());

			// Can early out if the node doesn't have the property 
			if (debugAttribute)
			{
				// Else check the properties value
				bool const debugPropertyValue = StringConverter::FromString<bool>(debugAttribute.value());
				return debugPropertyValue;
			}
		}

		return false;
	}

	MetaECSTypeData XmlParser::CreateECSTypeData(pugi::xml_node const& node) const NABI_NOEXCEPT
	{
		std::vector<PropertyData> componentProperties;
		for (pugi::xml_node const propertyNode : node.children())
		{
			// Check if the property is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(propertyNode))
			{
				PropertyData const propertyData = CreatePropertyData(propertyNode);
				componentProperties.push_back(propertyData);
			}
		}

		std::string const& nodeId = StringStore::Instance()->Add(node.attribute(c_IdAttribute.c_str()).value());

		MetaECSTypeData ecsTypeData; // Note - MetaECSTypeData can refer to a System or a Component
		ecsTypeData.m_Id = entt::hashed_string(nodeId.c_str());
		ecsTypeData.m_Properties = componentProperties;

		return ecsTypeData;
	}

	PropertyData XmlParser::CreatePropertyData(pugi::xml_node const& node) const NABI_NOEXCEPT
	{
		std::string const& nodeId = StringStore::Instance()->Add(node.attribute(c_IdAttribute.c_str()).value());
		std::string const& nodeValue = StringStore::Instance()->Add(node.attribute(c_ValueAttribute.c_str()).value());

		PropertyData propertyData;
		propertyData.m_Id = entt::hashed_string(nodeId.c_str());
		propertyData.m_Value = entt::hashed_string(nodeValue.c_str());

		return propertyData;
	}

	void nabi::Reflection::XmlParser::AddEntityTemplateToEntityGroupStore(std::string const& docGroup, EntityData const& entityData) NABI_NOEXCEPT
	{
		auto entityGroup = m_EntityGroups.find(docGroup);

		// Entity group already exists in the map
		if (entityGroup != m_EntityGroups.end())
		{
			std::vector<EntityData>& entitiesInGroup = entityGroup->second;
			entitiesInGroup.push_back(entityData);
		}
		// Entity group needs to be added to the map
		else
		{
			size_t constexpr entityTemplateGroupStartSize = 5u;

			std::vector<EntityData> newEntityGroup;
			newEntityGroup.reserve(entityTemplateGroupStartSize);

			newEntityGroup.push_back(entityData);

			auto const newGroupTemplatePair = std::make_pair(docGroup, newEntityGroup);
			m_EntityGroups.emplace(newGroupTemplatePair);
		}
	}
} // namespace nabi::Reflection
